export module Renderer;
import Shader;

export class Renderer
{
	void virtual DrawSquare(float width, float height);
	void virtual SetShader(Shader shader);
};