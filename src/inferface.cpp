include "../saba/src/Saba/Model/MMD/MMDModel.h"

// todo: 
void engine_load_model()
{
	std::shared_ptr<saba::MMDModel> mmdModel;
	std::string mmdDataPath = "";	// Set MMD data path(default toon texture path).
	std::string ext = saba::PathUtil::GetExt(modelPath);
	if (ext == "pmd")
	{
		auto pmdModel = std::make_unique<saba::PMDModel>();
		if (!pmdModel->Load(modelPath, mmdDataPath))
		{
			std::cout << "Load PMDModel Fail.\n";
			return false;
		}
		mmdModel = std::move(pmdModel);
}
else if (ext == "pmx")
{
	auto pmxModel = std::make_unique<saba::PMXModel>();
	if (!pmxModel->Load(modelPath, mmdDataPath))
	{
		std::cout << "Load PMXModel Fail.\n";
		return false;
	}
	mmdModel = std::move(pmxModel);
}
else
{
	std::cout << "Unsupported Model Ext : " << ext << "\n";
	return false;
}
}
void engine_load_animation()
{
	// Load animation.
auto vmdAnim = std::make_unique<saba::VMDAnimation>();
if (!vmdAnim->Create(mmdModel))
{
	std::cout << "Create VMDAnimation Fail.\n";
	return false;
}
for (const auto& vmdPath : vmdPaths)
{
	saba::VMDFile vmdFile;
	if (!saba::ReadVMDFile(&vmdFile, vmdPath.c_str()))
	{
		std::cout << "Read VMD File Fail.\n";
		return false;
	}
	if (!vmdAnim->Add(vmdFile))
	{
		std::cout << "Add VMDAnimation Fail.\n";
		return false;
	}
}
}

void engine_update_animation()
{
// Initialize pose.
{
	// Sync physics animation.
	mmdModel->InitializeAnimation();
	vmdAnim->SyncPhysics((float)animTime * 30.0f);
}

// Update animation(animation loop).
{
	// Update animation.
	mmdModel->BeginAnimation();
	// vmdAnim->Evaluate((float)animTime * 30.0f);
	// mmdModel->UpdateMorphAnimation(1.0f / 60.0f);
	// mmdModel->UpdateNodeAnimation(false);
	// mmdModel->UpdatePhysicsAnimation();
	// mmdModel->UpdateNodeAnimation(true);
	mmdModel->UpdateAllAnimation(vmdAnim.get(), (float)animTime * 30.0f, 1.0f / 60.0f);
	mmdModel->EndAnimation();

	// Update vertex.
	mmdModel->Update();
}
}