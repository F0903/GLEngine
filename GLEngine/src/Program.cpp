#include <string>
#include <iostream>
#include <vector>
#include <ctime>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture2D.h"
#include "Debug.h"

#define SBOX_IMGUI
//#define SBOX_SHADER_COUT

using uint = unsigned int;
template<class T> using Ref = std::shared_ptr<T>;

template<class T, class P = void> using Func = T(*)(P);

std::vector<const char*> windowTitles = std::vector<const char*>
{
	"SUMMON THE ELECTOR COUNTS!",
	"May the Cah be with thee",
	"DAVAI MACHIN",
	"Yeet a nibba skrr"
};

int buttonLabelIndex = 0;
std::vector<const char*> buttonLabels = std::vector<const char*>{ "Hello", "Ok", "pls", "k bye", "stahp" };

int viewport[2]{ 1000, 750 };
float aspect = float(viewport[0]) / float(viewport[1]);

void WindowResizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);

	viewport[0] = width;
	viewport[1] = height;
}

void GLDebugCallback(GLenum source,
					 GLenum type,
					 GLuint id,
					 GLenum severity,
					 GLsizei length,
					 const GLchar* message,
					 const void* userParam)
{
	bool pause = false;
	switch (severity)
	{
		case GL_DEBUG_SEVERITY_HIGH:
			std::cerr << "OPENGL ERROR (HIGH): " << message << '\n';
			pause = true;
			break;
		case GL_DEBUG_SEVERITY_MEDIUM:
			std::cerr << "OPENGL ERROR (MEDIUM): " << message << '\n';
			break;
		case GL_DEBUG_SEVERITY_LOW:
			std::cerr << "OPENGL ERROR (LOW): " << message << '\n';
			break;
		case GL_DEBUG_SEVERITY_NOTIFICATION:
			return;
		default:
			std::cerr << "OPENGL DEBUG (UNKNOWN): " << message << '\n';
			break;
	}
	if (pause)
	{
		std::cerr << "Severe error encountered. App has been paused. Please press enter to continue.";
		std::cin.get();
	}
}

int main()
{
	if (!glfwInit())
		return -1;

	srand(time(NULL));
	int ran = rand() % windowTitles.size();

#ifdef SBOX_DEBUG
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, 1);
#endif
	GLFWwindow* window = glfwCreateWindow(viewport[0], viewport[1], windowTitles[ran], NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetWindowSizeCallback(window, WindowResizeCallback);

	if (glewInit() != GLEW_OK)
		return -1;

#ifdef SBOX_IMGUI
	ImGui::CreateContext();
	auto& style = ImGui::GetStyle();
	style.FrameRounding = 6;
	style.ScrollbarRounding = 8;

	GLE_GL_DETECT_ERROR();

	ImGui_ImplOpenGL3_Init();
	ImGui_ImplGlfw_InitForOpenGL(window, false);
#endif // SBOX_IMGUI

	std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << '\n'
		<< "Device: " << glGetString(GL_RENDERER) << '\n' << '\n';

	auto vao = VertexBuffer{
		{{-1.0f, -1.0f}, {0.0f, 0.0f}},
		{{1.0f, -1.0f},  {1.0f, 0.0f}},
		{{1.0f, 1.0f},	 {1.0f, 1.0f}},
		{{-1.0f, 1.0f},  {0.0f, 1.0f}}
	};
	vao.Bind();
	GLE_GL_DETECT_ERROR();

	auto idb = IndexBuffer();
	idb.GenerateFromVerticies(vao.VertexCount());
	idb.Bind();
	GLE_GL_DETECT_ERROR();

	Shader basic("./assets/shader/Basic.shader");
	Shader rainbow("./assets/shader/Rainbow.shader");
	Shader* active = &basic;
	active->Bind();
	GLE_GL_DETECT_ERROR();

	Ref<Texture2D> tex;
	try
	{
		tex = Ref<Texture2D>(new Texture2D("./assets/media/cah64.png", TextureResizeMode::Nearest, TextureWrapMode::Repeat));
		tex->Bind();
		active->SetUniform("tex", tex->GetSlot());
		GLE_GL_DETECT_ERROR();
	}
	catch (std::exception ex)
	{
		std::cerr << "TEXTURE ERROR: " << ex.what() << '\n';
	}

#ifdef SBOX_DEBUG	
	glDebugMessageCallback(GLDebugCallback, nullptr);
#endif // SBOXDEBUG

	bool rbowShader = true;

	bool vsync = true;
	float fpsDelay = 0.1f;
	double preciseFps = 0;
	int roundedFps = 0;

	int frameCap = 24; //TODO: Make it do something
	bool preciseFrameCounter = false;
	double lastFrameTime = glfwGetTime();
	double lastFrameDelayTime = glfwGetTime();
	while (!glfwWindowShouldClose(window))
	{
		double currentFrameTime = glfwGetTime();

		if (glfwGetTime() > lastFrameDelayTime + fpsDelay)
		{
			preciseFps = 1 / (currentFrameTime - lastFrameTime);
			roundedFps = (int)preciseFps;
			lastFrameDelayTime = glfwGetTime();
		}
		lastFrameTime = currentFrameTime;

#ifdef SBOX_IMGUI
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
#endif // SBOX_IMGUI

		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		active->SetUniform("time", (float)currentFrameTime);
		active->SetUniform("res", viewport[0], viewport[1]);
		GLE_GL_DETECT_ERROR()

			glDrawElements(GL_TRIANGLES, idb.IndexCount(), GL_UNSIGNED_INT, nullptr);
		GLE_GL_DETECT_ERROR()

#ifdef SBOX_IMGUI
			if (ImGui::Begin("Debug Info"))
			{
				ImGui::Text("Yeet", "Skeet");
				if (ImGui::Button(buttonLabels[buttonLabelIndex], ImVec2(50, 35)))
				{
					buttonLabelIndex = buttonLabelIndex != buttonLabels.size() - 1 ? buttonLabelIndex + 1 : 0;
				}

				ImGui::Checkbox("Rainbow", &rbowShader);
				if (rbowShader)
				{
					active = &rainbow;
					active->Bind();
				}
				else
				{
					active = &basic;
					active->Bind();

					float cols[3];
					ImGui::ColorPicker3("Color Picker", cols);
					active->SetUniform("col", cols[0], cols[1], cols[2]);
				}

				if (ImGui::BeginChild("FPS Info", ImVec2(preciseFrameCounter ? 165 : 165, 105), true))
				{
					ImGui::Text((std::string("FPS: ") + (preciseFrameCounter ? std::to_string(preciseFps) : std::to_string(roundedFps))).c_str());
					preciseFrameCounter = ImGui::Button(preciseFrameCounter ? "Rounded" : "Precise", ImVec2(60, 25)) ? !preciseFrameCounter : preciseFrameCounter;
					ImGui::SliderFloat("Delay", &fpsDelay, 0, 2);
					ImGui::Checkbox("V-Sync", &vsync);
				}
				ImGui::EndChild();
			}
		ImGui::End();
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#endif // SBOX_IMGUI

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		glfwSwapInterval(vsync); //V-SYNC 

		/* Poll for and process events */
		glfwPollEvents();
	}
#ifdef SBOX_IMGUI
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	glfwTerminate();
	ImGui::DestroyContext();
#endif // SBOX_IMGUI	
}