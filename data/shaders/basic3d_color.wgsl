struct VSOut {
    @builtin(position) position : vec4f,
    @location(0) color : vec3f,
};

// The memory location of the uniform is given by a pair of a *bind group* and a *binding*
//@group(0) @binding(0) var<uniform> uMVP: mat4x4f;

@vertex
fn vs_main(@location(0) pos : vec3f, @location(1) color : vec3f) -> VSOut
{
	var out : VSOut;
	out.position = vec4f(pos, 1.0);
	out.color = color;

	return out;
}

@fragment
fn fs_main(out : VSOut) -> @location(0) vec4f
{
	return vec4f(out.color, 1.0);
}
