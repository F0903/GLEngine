SetTexture("./resources/cah.png");

function Update() -- DO NOT PUT SETTEXTURE IN UPDATE LOOP WILL CAUSE MEMORY LEAK
	DrawSquare(0, 0, "50%", "100%");
end