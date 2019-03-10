#include "assignmentF/AssignmentF.h"
#include "common/core.h"

std::shared_ptr<Camera> AssignmentF::CreateCamera() const
{
    const glm::vec2 resolution = GetImageOutputResolution();
    std::shared_ptr<Camera> camera = std::make_shared<PerspectiveCamera>(resolution.x / resolution.y, 30.f);
    camera->SetPosition(glm::vec3(0.f, 0.f, 0.f));
    return camera;
}



// 0 -- Naive.
// 1 -- BVH.
// 2 -- Grid.
#define ACCELERATION_TYPE 1

std::shared_ptr<Scene> AssignmentF::CreateScene() const
{
    std::shared_ptr<Scene> newScene = std::make_shared<Scene>();
    
    // Create scene
    std::shared_ptr<BlinnPhongMaterial> scene_material = std::make_shared<BlinnPhongMaterial>();
    
    std::vector<std::shared_ptr<aiMaterial>> scene_materials;
    std::vector<std::shared_ptr<MeshObject>> scene_objects = MeshLoader::LoadMesh("sword/scene.obj", &scene_materials);
    std::shared_ptr<SceneObject> sceneObject = std::make_shared<SceneObject>();
    
    for (size_t i = 0; i < scene_objects.size(); ++i) {
        std::shared_ptr<Material> materialCopy = scene_material->Clone();
        //FOr GRAYSCALE COMMENT OUT
        materialCopy->LoadMaterialFromAssimp(scene_materials[i]);
        //std::cout << materialCopy->GetBaseDiffuseReflection()[0] << " " << materialCopy->GetBaseSpecularReflection()[0] << std::endl;
        materialCopy->SetAmbient(glm::vec3(0.f,0.f,0.f));
        materialCopy->SetReflectivity(0.f);
        materialCopy->SetTransmittance(0.f);
        scene_objects[i]->SetMaterial(materialCopy);
        
        std::shared_ptr<SceneObject> sceneObject = std::make_shared<SceneObject>();
        sceneObject->AddMeshObject(scene_objects[i]);
        sceneObject->SetPosition(glm::vec3(0.f, -0.7f, -2.3f));
        sceneObject->Rotate(glm::vec3(0,1,0), -PI/2);
        
        sceneObject->CreateAccelerationData(AccelerationTypes::BVH);
        sceneObject->ConfigureAccelerationStructure([](AccelerationStructure* genericAccelerator) {
            BVHAcceleration* accelerator = dynamic_cast<BVHAcceleration*>(genericAccelerator);
            accelerator->SetMaximumChildren(2);
            accelerator->SetNodesOnLeaves(2);
        });
        
        sceneObject->ConfigureChildMeshAccelerationStructure([](AccelerationStructure* genericAccelerator) {
            BVHAcceleration* accelerator = dynamic_cast<BVHAcceleration*>(genericAccelerator);
            accelerator->SetMaximumChildren(2);
            accelerator->SetNodesOnLeaves(2);
        });
        newScene->AddSceneObject(sceneObject);
    }
    
    // Lights
    std::shared_ptr<AreaLight> sunLight1 = std::make_shared<AreaLight>(glm::vec2(0.5f, 0.5f));
    sunLight1->SetSamplerAttributes(glm::vec3(2.f, 2.f, 1.f), 2);
    //sunLight1->SetLightColor(.75f * glm::vec3(1.f, 0.77f, 0.411f));
    sunLight1->SetLightColor(.75f * glm::vec3(1.f, 1.f, 1.f));
    sunLight1->SetPosition(glm::vec3(0.f,0.5f,-1.0f));
    sunLight1->Rotate(glm::vec3(1.f,0.f,0.f), -PI/6);
    //sunLight1->Rotate(glm::vec3(0.f,1.f,0.f), PI/12);

#if ACCELERATION_TYPE == 0
    newScene->GenerateAccelerationData(AccelerationTypes::NONE);
#elif ACCELERATION_TYPE == 1
    newScene->GenerateAccelerationData(AccelerationTypes::BVH);
#else
    UniformGridAcceleration* accelerator = dynamic_cast<UniformGridAcceleration*>(newScene->GenerateAccelerationData(AccelerationTypes::UNIFORM_GRID));
    assert(accelerator);
    // Assignment 7 Part 2 TODO: Change the glm::ivec3(10, 10, 10) here.
    accelerator->SetSuggestedGridSize(glm::ivec3(10,10,10));
#endif    
    newScene->AddLight(sunLight1);
    
    // Sunlight purple
    std::shared_ptr<AreaLight> sunLight2 = std::make_shared<AreaLight>(glm::vec2(0.5f, 0.5f));
    sunLight2->SetSamplerAttributes(glm::vec3(2.f, 2.f, 1.f), 2);
    //sunLight2->SetLightColor(.75f * glm::vec3(1.f, 0.641f, 0.784f));
    sunLight2->SetLightColor(.75f * glm::vec3(1.f, 1.f, 1.f));
    sunLight2->SetPosition(glm::vec3(0.f,0.5f,-1.0f));
    sunLight2->Rotate(glm::vec3(1.f,0.f,0.f), -PI/6);
    //sunLight2->Rotate(glm::vec3(0.f,1.f,0.f), PI/12);
    
#if ACCELERATION_TYPE == 0
    newScene->GenerateAccelerationData(AccelerationTypes::NONE);
#elif ACCELERATION_TYPE == 1
    newScene->GenerateAccelerationData(AccelerationTypes::BVH);
#else
    UniformGridAcceleration* accelerator = dynamic_cast<UniformGridAcceleration*>(newScene->GenerateAccelerationData(AccelerationTypes::UNIFORM_GRID));
    assert(accelerator);
    // Assignment 7 Part 2 TODO: Change the glm::ivec3(10, 10, 10) here.
    accelerator->SetSuggestedGridSize(glm::ivec3(10,10,10));
#endif
    newScene->AddLight(sunLight2);
    
    /*
    // Backlight
    std::shared_ptr<AreaLight> backLight = std::make_shared<AreaLight>(glm::vec2(0.5f, 0.5f));
    backLight->SetSamplerAttributes(glm::vec3(2.f, 2.f, 1.f), 8);
    backLight->SetLightColor(glm::vec3(1.f, 1.f, 1.f));
    backLight->SetPosition(glm::vec3(0.f,0.5f,-1.0f));
    backLight->Rotate(glm::vec3(0.f,1.f,0.f), PI);
    */
    
#if ACCELERATION_TYPE == 0
    newScene->GenerateAccelerationData(AccelerationTypes::NONE);
#elif ACCELERATION_TYPE == 1
    newScene->GenerateAccelerationData(AccelerationTypes::BVH);
#else
    UniformGridAcceleration* accelerator = dynamic_cast<UniformGridAcceleration*>(newScene->GenerateAccelerationData(AccelerationTypes::UNIFORM_GRID));
    assert(accelerator);
    // Assignment 7 Part 2 TODO: Change the glm::ivec3(10, 10, 10) here.
    accelerator->SetSuggestedGridSize(glm::ivec3(10,10,10));
#endif
    newScene->AddLight(sunLight2);
    
    return newScene;

}

std::shared_ptr<ColorSampler> AssignmentF::CreateSampler() const
{
    std::shared_ptr<JitterColorSampler> jitter = std::make_shared<JitterColorSampler>();
    jitter->SetGridSize(glm::ivec3(1, 1, 1));
    return jitter;
}

std::shared_ptr<class Renderer> AssignmentF::CreateRenderer(std::shared_ptr<Scene> scene, std::shared_ptr<ColorSampler> sampler) const
{
    return std::make_shared<BackwardRenderer>(scene, sampler);
}

int AssignmentF::GetSamplesPerPixel() const
{
    return 1;
}

bool AssignmentF::NotifyNewPixelSample(glm::vec3 inputSampleColor, int sampleIndex)
{
    return true;
}

int AssignmentF::GetMaxReflectionBounces() const
{
    return 4;
}

int AssignmentF::GetMaxRefractionBounces() const
{
    return 6;
}

glm::vec2 AssignmentF::GetImageOutputResolution() const
{
    return glm::vec2(768.f, 512.f);
}
