#include "content_loader.h"
#include "core/GLCommon.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "scene.h"
#include "managers/TextureManager.h"
#include "utility/path.h"
#include "utility/assimp_glm.h"
#include <filesystem>
namespace content
{

void create_scene(aiNode* node, utl::vector<unsigned int>& mesh_ids, scene::scene& scene_data)
{
	if (  node -> mNumMeshes )
	{
		model::model _model;
		for ( unsigned int i = 0; i < node->mNumMeshes; ++i )
		{
			unsigned int mesh_id = mesh_ids[node->mMeshes[i]];
			_model._meshes.emplace_back(mesh_id);	
		}
		_model._local_model = AssimpGLMHelpers::ConvertMatrixToGLMFormat(node->mTransformation);
		unsigned int id = model::add_model(_model);
		scene_data._models.emplace_back(id);
		std::cout << "created model with id " << id << " and no of meshes: " << _model._meshes.size() << std::endl;
	}

	for ( unsigned int i = 0; i < node->mNumChildren; ++i )
		create_scene(node->mChildren[i], mesh_ids, scene_data);
}

utl::vector<std::string> create_and_reister_texture_type(aiMaterial* material, aiTextureType type, const std::string& dir_path)
{
	utl::vector<std::string> _textures;
	for ( unsigned int i = 0; i < material->GetTextureCount(type); ++i )	
	{
		aiString str;
		material->GetTexture(type, i, &str);	
		std::string path(str.C_Str());
		path = dir_path + path;

		std::cout << "texture path: " << path << std::endl;
		utl::normalize_path(path);

		std::string _extension = std::filesystem::path(path).extension();

		GLenum format;
		_extension == ".png" ? format = GL_RGBA : format = GL_RGB;
		textures::add_texture(path, format, format);
		_textures.emplace_back(path);
	}
	return _textures;
}

mesh::material create_and_register_material(aiMaterial* material,const std::string& path)
{
	mesh::material material_data;
	material_data._textures_map = create_and_reister_texture_type(material, aiTextureType_DIFFUSE, path);
	material_data._specular_map = create_and_reister_texture_type(material, aiTextureType_SPECULAR, path);	

	return material_data;
}

unsigned int create_and_register_mesh(aiMesh* mesh,utl::vector<mesh::material>& materials)
{
	mesh::mesh mesh_data;
	std::cout << "started creating mesh " << std::endl;	
	for ( unsigned int i = 0; i < mesh->mNumVertices; ++i )
	{
      		vertex vertex_data;
		vertex_data.position.x = mesh->mVertices[i].x;
		vertex_data.position.y = mesh->mVertices[i].y;
		vertex_data.position.z = mesh->mVertices[i].z;
		
		vertex_data.normal.x = mesh->mNormals[i].x;
		vertex_data.normal.y = mesh->mNormals[i].y;
		vertex_data.normal.z = mesh->mNormals[i].z;

		vertex_data.uv = {0.f, 0.f};
		if ( mesh->mTextureCoords[0] ){
			vertex_data.uv.x = mesh->mTextureCoords[0][i].x;
			vertex_data.uv.y = mesh->mTextureCoords[0][i].y;
		}
		mesh_data._vertices.emplace_back(vertex_data);
	}

	std::cout << "created mesh" << std::endl;
	for ( unsigned int i = 0; i < mesh->mNumFaces; ++i )
	{
		aiFace face = mesh->mFaces[i];
		for ( unsigned int j = 0; j < face.mNumIndices; ++j )
			mesh_data._indices.emplace_back(face.mIndices[j]);
	}

	assert(mesh->mMaterialIndex < materials.size());
	

	mesh_data._material = materials[mesh->mMaterialIndex];
	
	unsigned int id = mesh::add_mesh(mesh_data);
	return id;
}

scene::scene create_scene_from_file(std::string path, bool uv_flipped)
{

	std::cout << "DEBUG loading scene" << std::endl;
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path.c_str(),aiProcess_JoinIdenticalVertices | aiProcess_Triangulate | aiProcess_ValidateDataStructure | (aiProcess_FlipUVs * uv_flipped));
	
	utl::vector<mesh::material> materials;
	std::filesystem::path fp(path);
	fp.remove_filename();

	for ( unsigned int i = 0; i < scene->mNumMaterials; ++i)
		materials.emplace_back(create_and_register_material(scene->mMaterials[i], fp.string()));

	utl::vector<unsigned int> mesh_ids;
	
	for ( unsigned int i = 0; i < scene->mNumMeshes; ++i )
		mesh_ids.emplace_back(create_and_register_mesh(scene->mMeshes[i], materials));

	std::cout << "got here" << std::endl;
	scene::scene scene_data;
	create_scene(scene->mRootNode, mesh_ids, scene_data);
	std::cout << "DREW SCENE" << std::endl;
	return scene_data;
}	

}
