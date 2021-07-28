cah = LoadTexture("./resources/cah.png");
alabama = LoadTexture("./resources/alabama.png");

function Update() -- DO NOT PUT LOADTEXTURE IN UPDATE LOOP WILL CAUSE MEMORY LEAK
	SetTexture(cah);
	DrawSquare(0, 0, "50%", "100%");
	SetTexture(alabama);
	DrawSquare("100%", 0, "50%", "100%");
end