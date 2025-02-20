// The memory location of the uniform is given by a pair of a *bind group* and a *binding*
@group(0) @binding(0) var<uniform> uMVP: mat4x4f;

@vertex
fn vs_main(@location(0) pos: vec3f) -> @builtin(position) vec4f
{
	return vec4f(pos, 1.0);
}

@fragment
fn fs_main() -> @location(0) vec4f
{
	return vec4f(1.0, 1.0, 0.0, 1.0);
}
