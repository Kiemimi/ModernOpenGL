#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>
#include <string>
#include <vector>

using std::string;
using std::vector;

unsigned int TextureFromFile(const char* path, const string& directory, bool gamma = false);

class Model
{
	public:
		vector<Mesh> meshes;
		vector<Texture> textures_loaded;
		string directory;

		Model(char* path) {
			loadModel(path);
		}
		void Draw(Shader& shader) {
			for (unsigned int i = 0; i < meshes.size(); i++)
				meshes[i].Draw(shader);
		}
	private:
		void loadModel(string const& path);
		void processNode(aiNode* node, const aiScene* scene);
		Mesh processMesh(aiMesh* mesh, const aiScene* scene);
		vector<Texture> loadMaterialTextures(aiMaterial * mat, aiTextureType type, string typeName); 
};