struct VSOut {
    @builtin(position) position : vec4f,
    @location(0) color : vec3f,
};

@vertex
fn vs_main(@location(0) pos : vec3f, @location(1) color : vec3f) -> VSOut
{
	var vsOut : VSOut;
	vsOut.position = vec4f(pos, 1.0);
	vsOut.color = color;

	return vsOut;
}

@fragment
fn fs_main(vsOut : VSOut) -> @location(0) vec4f
{
	return vec4f(vsOut.color, 1.0);
}
