#include "WindowManager.h"

_NL::Engine::WindowManager::WindowManager(const char* WindowName, int Width, int height, bool fullscreen)
{
	window = new sf::RenderWindow(sf::VideoMode(Width, height), WindowName);
	//SET FULLSCREEN
	if (fullscreen) {
		window->create(sf::VideoMode::getFullscreenModes()[0], WindowName, sf::Style::Fullscreen);
	}
	glewInit();
	
}

void _NL::Engine::WindowManager::RunGameLoop()
{
	Start();
	while (window->isOpen()) {
		update();
	}
	
}

void _NL::Engine::WindowManager::OpenGLStart()
{
	//GET OBJ DATA from WORLD
	for each (_NL::Object::GameObject* obj in CurrentScene->GetObjectList())
	{
		if (obj->ClassName() == "_NL::Object::GameObject") {
			std::cout << obj->name << std::endl;
			MeshDataStack.push_back(&obj->getComponent(_NL::Component::MeshRenderer())->Mesh->MeshData);
			MeshElementsStack.push_back(&obj->getComponent(_NL::Component::MeshRenderer())->Mesh->Indices);
		}	
	}

	///UNPACK VERTEX DATA
	for each (_NL::Core::MeshData* Obj in MeshDataStack)
	{
		for each(_NL::Core::VertexPos P in Obj->vPos) {
			VertsBuf.push_back(P.Pos.x);
			VertsBuf.push_back(P.Pos.y);
			VertsBuf.push_back(P.Pos.z);
		}
		for each(_NL::Core::VertexCol C in Obj->vCol) {
			VertsBuf.push_back(C.Col.r);
			VertsBuf.push_back(C.Col.g);
			VertsBuf.push_back(C.Col.b);
		}
		for each(_NL::Core::VertexNorm N in Obj->vNorm) {
			VertsBuf.push_back(N.Norm.x);
			VertsBuf.push_back(N.Norm.y);
			VertsBuf.push_back(N.Norm.z);
		}
		for each(_NL::Core::VertexTexCoord T in Obj->vTexC) {
			VertsBuf.push_back(T.TexCoord.x);
			VertsBuf.push_back(T.TexCoord.y);
		}
	}

	///UNPACK INDEX DATA
	for each (std::vector<_NL::Core::vIndices>* Obj in MeshElementsStack)
	{
		for each (_NL::Core::vIndices I in *Obj)
		{
			IndicesBuf.push_back(I.v [0]);
			IndicesBuf.push_back(I.vt[0]);
			IndicesBuf.push_back(I.vn[0]);
			IndicesBuf.push_back(I.v [1]);
			IndicesBuf.push_back(I.vt[1]);
			IndicesBuf.push_back(I.vn[1]);
			IndicesBuf.push_back(I.v [2]);
			IndicesBuf.push_back(I.vt[2]);
			IndicesBuf.push_back(I.vn[2]);
		}
	}

	MeshDataStack.clear();
	MeshElementsStack.clear();

	///GL BUFFERS && ARRAYS

	glCreateVertexArrays(1, &vao);
	glEnableVertexArrayAttrib(vao, Pos_atrib);
	glEnableVertexArrayAttrib(vao, Col_atrib);
	glEnableVertexArrayAttrib(vao, Norm_atrib);
	glEnableVertexArrayAttrib(vao, TexC_atrib);
	check_gl_error();

	glVertexArrayAttribBinding(vao, Pos_atrib, 0);
	glVertexArrayAttribFormat(vao, Pos_atrib,	3, GL_FLOAT, false, sizeof(GL_FLOAT) * 0);

	glVertexArrayAttribBinding(vao, Col_atrib, 0);
	glVertexArrayAttribFormat(vao, Col_atrib,	3, GL_FLOAT, false, sizeof(GL_FLOAT) * 3);

	glVertexArrayAttribBinding(vao, Norm_atrib, 0);
	glVertexArrayAttribFormat(vao, Norm_atrib,	3, GL_FLOAT, false, sizeof(GL_FLOAT) * 6);

	glVertexArrayAttribBinding(vao, TexC_atrib, 0);
	glVertexArrayAttribFormat(vao, TexC_atrib,	2, GL_FLOAT, false, sizeof(GL_FLOAT) * 9);
	
	check_gl_error();

	///DYNAMIC VERTEX BUFFER OBJ
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertsBuf), &VertsBuf, GL_STATIC_DRAW);
	///DYNAMIC ELEMENT BUFFER OBJ 
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(IndicesBuf), &IndicesBuf, GL_STATIC_DRAW);
	
	check_gl_error();

	glVertexArrayVertexBuffer(vao, 0, vbo, 0, sizeof(GL_FLOAT) * 11);
	glVertexArrayElementBuffer(vao, ebo);
	
	check_gl_error();

	/*glBindVertexArray(vao);
	glDrawArrays();*/
	
}

void _NL::Engine::WindowManager::Start()
{
	OpenGLStart();
}


void _NL::Engine::WindowManager::update() {
	
	sf::Event event;
	while (window->pollEvent(event))
	{
		//INPUT HANDLER
		if (event.type == sf::Event::Closed)
			window->close();
		//...//
	}
	//UPDATE DISPLAY;
	window->display();
	//...//
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, IndicesBuf.size() / 3, GL_UNSIGNED_INT, 0);
}


_NL::Engine::WindowManager::~WindowManager()
{
}




