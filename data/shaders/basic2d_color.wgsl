struct VSOut {
    @builtin(position) position : vec4f,
    @location(0) color : vec3f,
};

@vertex
fn vs_main(@location(0) pos: vec2f,  @location(1) color : vec3f) -> VSOut
{
	var out : VSOut;
	out.position = vec4f(pos, 0.0, 1.0);
	out.color = color;
	
	return out;
}

@fragment
fn fs_main(out : VSOut) -> @location(0) vec4f
{
	return vec4f(out.color, 1.0);
}
