#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

#include "planet.hpp"
#include "shader.hpp"
#include "state.hpp"
#include "camera.hpp"

GLuint loadTexture(const char* texImagePath) {
	GLuint textureID;
	textureID = SOIL_load_OGL_texture(texImagePath, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
	if (textureID == 0)
		printf("could not find texture file\n");
	return textureID;
}

void mouse_callback(GLFWwindow* window, double x, double y);

void scroll_callback(GLFWwindow* window, double, double delta_scroll);

void key_callback(GLFWwindow* window, int key, int, int action, int);

void render_satellites_of_satellite(Planet planet, Shader shader, glm::mat4 proj, glm::mat4 view, glm::mat4 model) {
	for (int i = 0; i < planet.satellites.size(); i++) {
		Planet satellite = planet.satellites.at(i);
		model = glm::rotate(model, 8 * (float)glfwGetTime() / satellite.distance, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(satellite.distance, 0.0f, satellite.distance));
		model = glm::scale(model, satellite.scale);
		shader.set_mvp(proj * view * model);
		satellite.render();
	}
}

void render_satellites(Planet planet, Shader shader, glm::mat4 proj, glm::mat4 view) {
	for (int i = 0; i < planet.satellites.size(); i++) {
		Planet satellite = planet.satellites.at(i);
		glm::mat4 model = glm::mat4{ 1.0f };
		model = glm::translate(model, planet.position);
		model = glm::rotate(model, 8 * (float)glfwGetTime() / satellite.distance, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(satellite.distance, 0.0f, satellite.distance));
		model = glm::scale(model, satellite.scale);
		shader.set_mvp(proj * view * model);
		satellite.render();
		if (satellite.satellites.size() != 0)render_satellites_of_satellite(satellite, shader, proj, view, model);
	}
}

int main() {
	if (!glfwInit()) {
		std::cerr << "failed to initialize GLFW" << std::endl;
		return 1;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Solar System", nullptr, nullptr);
	if (!window) {
		std::cerr << "failed to create GLFW window" << std::endl;
		glfwTerminate();
		return 1;
	}

	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK) {
		std::cerr << "failed to initialize GLEW" << std::endl;
		return 1;
	}

	// aci setam ce callbackuri se apeleaza cand se misca mouseu, scrollu sau se apasa pe taste
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetKeyCallback(window, key_callback);
	// asta face cursoru sa dispara si sa ramana in fereastra. dai exit sa iesi
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	State  state{};
	Camera camera{ { 0.0f, 0.0f, 0.0f } };
	// ii dam la window app-stateu, ca sa poata fi accesat in callbackuri
	glfwSetWindowUserPointer(window, &state);

	// vector de planete. in viitor cel mai probabil aici o sa initializam doar gen soarele sau ce o fi, la care ii adaugam dupa planetele
	std::vector<Planet> planets;
	float baseSize = 0.2f;
	float baseDistance = 6.0f;
	Planet sun = Planet(glm::vec3{ 0.0f, 0.0f, 0.0f }, glm::vec3{ 8.0f });
	sun.texture = loadTexture("res/sun.jpg");
	sun.distance = 0.0f * baseDistance;
	Planet mercury = Planet(glm::vec3{ 0.0f, 0.0f, 0.0f }, glm::vec3{ baseSize * 1.0f });
	mercury.texture = loadTexture("res/mercury.jpg");
	mercury.distance = 1.0f * baseDistance;
	Planet venus = Planet(glm::vec3{ 0.0f, 0.0f, 0.0f }, glm::vec3{ baseSize * 1.15f });
	venus.texture = loadTexture("res/venus.jpg");
	venus.distance = 1.09f * baseDistance;
	Planet earth = Planet(glm::vec3{ 10.0f, 0.0f, 0.0f }, glm::vec3{ baseSize * 1.16f });
	earth.texture = loadTexture("res/earth.jpg");
	earth.distance = 1.16f * baseDistance;
	Planet moon = Planet(glm::vec3{ 0.0f, 0.0f, 0.0f }, glm::vec3{ baseSize * 1.0f });
	moon.texture = loadTexture("res/moon.jpg");
	moon.distance = 0.155f * baseDistance;
	Planet mars = Planet(glm::vec3{ 0.0f, 0.0f, 0.0f }, glm::vec3{ baseSize * 1.04f });
	mars.texture = loadTexture("res/mars.png");
	mars.distance = 1.30f * baseDistance;
	Planet jupiter = Planet(glm::vec3{ 0.0f, 0.0f, 0.0f }, glm::vec3{ baseSize * 3.81f });
	jupiter.texture = loadTexture("res/jupiter.jpg");
	jupiter.distance = 2.27f * baseDistance;
	Planet saturn = Planet(glm::vec3{ 0.0f, 0.0f, 0.0f }, glm::vec3{ baseSize * 3.33f });
	saturn.texture = loadTexture("res/saturn.jpg");
	saturn.distance = 3.41f * baseDistance;
	Planet uranus = Planet(glm::vec3{ 0.0f, 0.0f, 0.0f }, glm::vec3{ baseSize * 1.95f });
	uranus.texture = loadTexture("res/uranus.jpg");
	uranus.distance = 5.95f * baseDistance;
	Planet neptune = Planet(glm::vec3{ 0.0f, 0.f, 0.0f }, glm::vec3{ baseSize * 1.93f });
	neptune.texture = loadTexture("res/neptune.jpg");
	neptune.distance = 8.79f * baseDistance;
	Planet pluto = Planet(glm::vec3{ 0.0f, 0.0f, 0.0f }, glm::vec3{ baseSize * 0.95f });
	pluto.texture = loadTexture("res/pluto.jpg");
	pluto.distance = 11.25f * baseDistance;

	earth.addPlanet(moon);
	sun.addPlanet(mercury);
	sun.addPlanet(venus);
	sun.addPlanet(earth);
	sun.addPlanet(mars);
	sun.addPlanet(jupiter);
	sun.addPlanet(saturn);
	sun.addPlanet(uranus);
	sun.addPlanet(neptune);
	sun.addPlanet(pluto);

	planets.emplace_back(sun);

	// unicu shader. poate o sa mai avem si altele
	Shader shader{ "shaders/vertex.glsl", "shaders/fragment.glsl" };
	glUseProgram(shader.id);

	glEnable(GL_DEPTH_TEST);
	while (!glfwWindowShouldClose(window)) {
		// asta calculeaza cat timp se scurge intre cadre, ca sa poti sa te folosesti de asta cand misti camera si sa fie mai smooth
		auto time = (float)glfwGetTime();
		state.delta.time = time - state.last_time;
		state.last_time = time;

		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// updatam camera in functie de ce chestii face useru
		camera.update(state.held_keys, state.delta);

		glm::mat4 proj{ glm::perspective(glm::radians(camera.fov), (float)WIDTH / HEIGHT, 0.1f, 100.0f) };
		glm::mat4 view{ camera.view_matrix() };

		// iteram prin obiecte si le calculam la fiecare model matrixu in functie de pozitia si marimea lor, le trimitem la shader, si le dam draw
		for (const auto& planet : planets) {
			glm::mat4 model{ 1.0f };
			model = glm::translate(model, planet.position);
			model = glm::scale(model, planet.scale);
			shader.set_mvp(proj * view * model);
			planet.render();
			render_satellites(planet, shader, proj, view);
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void mouse_callback(GLFWwindow* window, double x, double y) {
	auto state = (State*)glfwGetWindowUserPointer(window);

	// astea trebe pentru ca GLFW iti da pozitia curenta a mouseului in fereastra, da pe camera o intereseaza cat si cum s-o miscat mouseu fata de unde era data trecuta, nu fix unde e acuma
	state->delta.mouse_x = (float)(x - state->last_mouse_x);
	state->delta.mouse_y = (float)(state->last_mouse_y - y);

	state->last_mouse_x = x;
	state->last_mouse_y = y;
}

void scroll_callback(GLFWwindow* window, double, double delta_scroll) {
	auto state = (State*)glfwGetWindowUserPointer(window);
	state->delta.mouse_scroll = (float)delta_scroll;
}

void key_callback(GLFWwindow* window, int key, int, int action, int) {
	auto state = (State*)glfwGetWindowUserPointer(window);

	if (action == GLFW_PRESS) {
		switch (key) {
			// apesti ESC, se opreste
		case GLFW_KEY_ESCAPE: glfwSetWindowShouldClose(window, true);
			break;
			// setam care keys is apasate
		case GLFW_KEY_W: state->held_keys[State::W] = true;
			break;
		case GLFW_KEY_A: state->held_keys[State::A] = true;
			break;
		case GLFW_KEY_S: state->held_keys[State::S] = true;
			break;
		case GLFW_KEY_D: state->held_keys[State::D] = true;
			break;
		default: break;
		}
	}
	// setam care keys nu mai is apasate
	if (action == GLFW_RELEASE) {
		switch (key) {
		case GLFW_KEY_W: state->held_keys[State::W] = false;
			break;
		case GLFW_KEY_A: state->held_keys[State::A] = false;
			break;
		case GLFW_KEY_S: state->held_keys[State::S] = false;
			break;
		case GLFW_KEY_D: state->held_keys[State::D] = false;
			break;
		default: break;
		}
	}
}
