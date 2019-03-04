#include "Convert.h"

int Convert(char* filepath) {
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filepath, My);
	if (!scene) {
		fprintf(stderr, importer.GetErrorString());
		std::cin.get();
		return -1;
	}

	std::cout << "Loading " << scene->mNumMeshes << " meshes" << std::endl;

	std::string l_name = filepath;

	{
		unsigned int i;
		for (i = 0; i < l_name.size(); i++)
			if (l_name[i] == '.') break;
		l_name.erase(i, l_name.size());
		l_name.append(".obt");
	}

	FILE* file = fopen(l_name.c_str(), "wb");
	// Write the amount of meshes
	fwrite(&scene->mNumMeshes, sizeof(int), 1, file);
	fclose(file);

	// Write each mesh
	for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
		std::string f_name = l_name;
		f_name.append(std::to_string(i));
		std::cout << f_name.c_str() << std::endl;
		FILE* m_file = fopen(f_name.c_str(), "wb");
		if (!m_file) {
			std::cout << "File problem\n";
			std::cin.get();
			return -1;
		}

		// Write the amout of vertices
		const aiMesh* mesh = scene->mMeshes[i];
		std::cout << "Mesh: " << i << "\n";

		glm::vec3 info = { mesh->mNumVertices, mesh->mNumFaces * 3, INT16_MAX };
		fwrite(&info, sizeof(glm::vec3), 1, m_file);
		glm::vec3 info2 = { (unsigned int)mesh->HasNormals(), (unsigned int)mesh->HasTextureCoords(0), 0 };
		fwrite(&info2, sizeof(glm::vec3), 1, m_file);

		std::cout << "info: " << info.x << " " << info.y << " " << info.z << "\n";
		std::cout << "NumVertices: " << info.x << "\n";

		// Write mesh position
		glm::vec3 pos;
		pos.x = scene->mRootNode[0].mChildren[i]->mTransformation.a4 / 10000;
		pos.y = scene->mRootNode[0].mChildren[i]->mTransformation.b4 / 10000;
		pos.z = scene->mRootNode[0].mChildren[i]->mTransformation.c4 / 10000;
		fwrite(&pos, sizeof(glm::vec3), 1, m_file);
		std::cout << "Pos: " << pos.x << " " << pos.y << " " << pos.z << "\n";

		// Write Vertices
		std::cout << "Writing vertices\n";
		glm::vec3* vertices = new glm::vec3[info.x];
		for (int j = 0; j < info.x; j++) {
			vertices[j].x = static_cast<float>(mesh->mVertices[j].x);
			vertices[j].y = static_cast<float>(mesh->mVertices[j].y);
			vertices[j].z = static_cast<float>(mesh->mVertices[j].z);
			//std::cout << "Vertices: " << vertices[j].x << " " << vertices[j].y << " " << vertices[j].z << "\n";
		}
		fwrite(&vertices[0], sizeof(glm::vec3), info.x, m_file);

		std::cout << "Writing normals\n";
		// Write Normals
		glm::vec3* normals = new glm::vec3[info.x];
		for (int j = 0; j < info.x; j++) {
			normals[j].x = mesh->mVertices[j].x;
			normals[j].y = mesh->mVertices[j].y;
			normals[j].z = mesh->mVertices[j].z;
		}
		fwrite(&normals[0], sizeof(glm::vec3), info.x, m_file);

		// Write Uvs
		if (mesh->HasTextureCoords(0)) {
			std::cout << "Writing textures\n";
			glm::vec2* uv = new glm::vec2[info.x];
			for (unsigned int j = 0; j < info.x; j++) {
				uv[j].x = mesh->mTextureCoords[0][j].x;
				uv[j].y = mesh->mTextureCoords[0][j].y;
				//std::cout << uv[j].x << " " << uv[j].y << "\n";
			}
			//std::cout << uv[0].x << " " << uv[0].y << std::endl;
			fwrite(&uv[0], sizeof(glm::vec2), info.x, m_file);
		}

		// Write Indices
		std::cout << "Writing indices\n";
		struct Index {
			unsigned int x, y, z;
		};
		Index* indices = new Index[mesh->mNumFaces * 3];
		std::cout << "Indices " << mesh->mNumFaces * 3 << "\n";
		for (int j = 0; j < mesh->mNumFaces; j++) {
			indices[j].x = static_cast<unsigned int>(mesh->mFaces[j].mIndices[0]);
			indices[j].y = static_cast<unsigned int>(mesh->mFaces[j].mIndices[1]);
			indices[j].z = static_cast<unsigned int>(mesh->mFaces[j].mIndices[2]);
			//std::cout << "Indices: " << indices[j].x << " " << indices[j].y << " " << indices[j].z << "\n";
		}
		fwrite(&indices[0], sizeof(Index), mesh->mNumFaces * 3, m_file);
		fclose(m_file);
	}
}
