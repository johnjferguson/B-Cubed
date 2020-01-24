#include "Physics.h"
#include "Gui.h"
#include <sstream>

using namespace physx;

Physics::Physics()
{
	gFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gAllocator, gErrorCallback);
	gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale(), true);

	PxCudaContextManagerDesc cudaContextManagerDesc;

#ifdef RENDER_SNIPPET
	cudaContextManagerDesc.interopMode = PxCudaInteropMode::OGL_INTEROP;	//Choose interop mode. As the snippets use OGL, we select OGL_INTEROP
																			//when using D3D, cudaContextManagerDesc.graphicsDevice must be set as the graphics device pointer.
#else
	cudaContextManagerDesc.interopMode = PxCudaInteropMode::NO_INTEROP;
#endif

	gCudaContextManager = PxCreateCudaContextManager(*gFoundation, cudaContextManagerDesc, PxGetProfilerCallback());	//Create the CUDA context manager, required for GRB to dispatch CUDA kernels.
	if (gCudaContextManager)
	{
		if (!gCudaContextManager->contextIsValid())
		{
			gCudaContextManager->release();
			gCudaContextManager = NULL;
		}
	}

	PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
	gDispatcher = PxDefaultCpuDispatcherCreate(4);			//Create a CPU dispatcher using 4 worther threads
	sceneDesc.cpuDispatcher = gDispatcher;
	sceneDesc.filterShader = PxDefaultSimulationFilterShader;

	sceneDesc.cudaContextManager = gCudaContextManager;		//Set the CUDA context manager, used by GRB.

	sceneDesc.flags |= PxSceneFlag::eENABLE_GPU_DYNAMICS;	//Enable GPU dynamics - without this enabled, simulation (contact gen and solver) will run on the CPU.
	sceneDesc.flags |= PxSceneFlag::eENABLE_PCM;			//Enable PCM. PCM NP is supported on GPU. Legacy contact gen will fall back to CPU
	sceneDesc.flags |= PxSceneFlag::eENABLE_STABILIZATION;	//Improve solver stability by enabling post-stabilization.
	sceneDesc.broadPhaseType = PxBroadPhaseType::eGPU;		//Enable GPU broad phase. Without this set, broad phase will run on the CPU.
	sceneDesc.gpuMaxNumPartitions = 8;						//Defines the maximum number of partitions used by the solver. Only power-of-2 values are valid. 
															//A value of 8 generally gives best balance between performance and stability.
	gScene = gPhysics->createScene(sceneDesc);

	gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);

	PxRigidStatic* groundPlane = PxCreatePlane(*gPhysics, PxPlane(0, 1, 0, 0), *gMaterial);
	gScene->addActor(*groundPlane);

	ball = CreateDynamic(PxTransform(PxVec3(0, 20, 100)), PxSphereGeometry(5), PxVec3(0, -25, -100));
	PxRigidBodyExt::updateMassAndInertia(*ball, 1000.f);
}

Physics::~Physics()
{
	gFoundation->release();
	gDispatcher->release();
	gScene->release();
	gMaterial->release();
	//gCudaContextManager->release();
	ball->release();
}

void Physics::Update(Time dt)
{
	gScene->simulate(dt.Seconds());
	gScene->fetchResults(true);
	PxVec3 vel = ball->getLinearVelocity();
	std::stringstream ss;
	ss << "Velocity of ball (x,y,z) = (" << (abs(vel.x) < 0.1 ? 0.0f : vel.x) << "," << (abs(vel.y) < 0.1 ? 0.0f : vel.y) << "," << (abs(vel.z) < 0.1 ? 0.0f : vel.z) << ")";
	Gui::AddText(ss.str());
}

PxRigidDynamic* Physics::CreateDynamic(const physx::PxTransform & t, const physx::PxGeometry & geometry, const physx::PxVec3& velocity)
{
	PxRigidDynamic* dynamic = PxCreateDynamic(*gPhysics, t, geometry, *gMaterial, 10.0f);
	dynamic->setAngularDamping(0.5f);
	dynamic->setLinearVelocity(velocity);
	gScene->addActor(*dynamic);
	return dynamic;
}


