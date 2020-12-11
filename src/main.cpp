#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

#include "planet.hpp"
#include "shader.hpp"
#include "state.hpp"
#include "camera.hpp"

void mouse_callback(GLFWwindow* window, double x, double y);

void scroll_callback(GLFWwindow* window, double, double delta_scroll);

void key_callback(GLFWwindow* window, int key, int, int action, int);

float base_size = 0.2f;
float base_distance = 6.0f;

void render_satellites_of_satellite(Planet planet, Shader shader, glm::mat4 proj, glm::mat4 view, glm::mat4 model) {
	for (int i = 0; i < planet.satellites.size(); i++) {
		Planet satellite = planet.satellites.at(i);
		model = glm::rotate(model, satellite.rotation_speed * (float)glfwGetTime() / (satellite.distance * base_distance), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(satellite.distance * base_distance, 0.0f, satellite.distance * base_distance));
		model = glm::scale(model, satellite.scale * base_size);
		shader.set_mvp(proj * view * model);
		satellite.render();
	}
}

void render_satellites(Planet planet, Shader shader, glm::mat4 proj, glm::mat4 view) {
	for (int i = 0; i < planet.satellites.size(); i++) {
		Planet satellite = planet.satellites.at(i);
		glm::mat4 model = glm::mat4{ 1.0f };
		model = glm::translate(model, planet.position);
		model = glm::rotate(model, satellite.rotation_speed * (float)glfwGetTime() / (satellite.distance * base_distance), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(satellite.distance * base_distance, 0.0f, satellite.distance * base_distance));
		model = glm::scale(model, satellite.scale * base_size);
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

	if (glewIsSupported("GL_EXT_texture_filter_anisotropic"))
	{
		GLfloat anisoSetting = 0.0f;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &anisoSetting);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisoSetting);
	}

	// vector de planete. in viitor cel mai probabil aici o sa initializam doar gen soarele sau ce o fi, la care ii adaugam dupa planetele
	std::vector<Planet> planets;
	Planet sun = Planet(glm::vec3{ 0.0f, 0.0f, 0.0f }, glm::vec3{ 8.0f }, "res/sun.jpg", 0.0f, 8);
	Planet mercury = Planet(glm::vec3{ 0.0f, 0.0f, 0.0f }, glm::vec3{ 1.0f }, "res/mercury.jpg", 1.0f, 8);
	Planet venus = Planet(glm::vec3{ 0.0f, 0.0f, 0.0f }, glm::vec3{ 1.15f }, "res/venus.jpg", 1.09f, 8);
	Planet earth = Planet(glm::vec3{ 0.0f, 0.0f, 0.0f }, glm::vec3{ 1.16f }, "res/earth.jpg", 1.16f, 8);
	Planet moon = Planet(glm::vec3{ 0.0f, 0.0f, 0.0f }, glm::vec3{ 1.0f }, "res/moon.jpg", 0.155f, 8);
	Planet mars = Planet(glm::vec3{ 0.0f, 0.0f, 0.0f }, glm::vec3{ 1.04f }, "res/mars.png", 1.30f, 8);
	Planet jupiter = Planet(glm::vec3{ 0.0f, 0.0f, 0.0f }, glm::vec3{ 3.81f }, "res/jupiter.jpg", 2.27f, 8);
	Planet saturn = Planet(glm::vec3{ 0.0f, 0.0f, 0.0f }, glm::vec3{ 3.33f }, "res/saturn.jpg", 3.41f, 8);
	Planet uranus = Planet(glm::vec3{ 0.0f, 0.0f, 0.0f }, glm::vec3{ 1.95f }, "res/uranus.jpg", 5.95f, 8);
	Planet neptune = Planet(glm::vec3{ 0.0f, 0.f, 0.0f }, glm::vec3{ 1.93f }, "res/neptune.jpg", 8.79f, 8);
	Planet pluto = Planet(glm::vec3{ 0.0f, 0.0f, 0.0f }, glm::vec3{ 0.95f }, "res/pluto.jpg", 11.25f, 8);

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
