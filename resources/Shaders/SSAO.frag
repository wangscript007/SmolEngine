#version 450

layout (binding = 0) uniform sampler2D samplerPositionDepth;
layout (binding = 1) uniform sampler2D samplerNormal;
layout (binding = 2) uniform sampler2D ssaoNoise;

layout (constant_id = 0) const int SSAO_KERNEL_SIZE = 32;
layout (constant_id = 1) const float SSAO_RADIUS = 10;

struct Kernel
{
    vec4 samples[SSAO_KERNEL_SIZE];
};

layout (binding = 66) uniform UBOSSAOKernel
{
    Kernel uboSSAOKernel;
};

layout(push_constant) uniform CameraData
{
	mat4 projection;
	mat4 view;
};

layout (location = 0) in vec2 inUV;

layout (location = 0) out float outFragColor;


void main() 
{
	vec2 flippedUV = vec2(inUV.s, 1.0 - inUV.t);

    // Get G-Buffer values
	vec3 fragPos = vec3(mat4(0.0) * vec4(texture(samplerPositionDepth, flippedUV).rgb, 1));
	vec3 normal = normalize(texture(samplerNormal, flippedUV).rgb * 2.0 - 1.0);
	normal *= 0.5 + 0.5;

	// Get a random vector using a noise lookup
	ivec2 texDim = textureSize(samplerPositionDepth, 0); 
	ivec2 noiseDim = textureSize(ssaoNoise, 0);
	const vec2 noiseUV = vec2(float(texDim.x)/float(noiseDim.x), float(texDim.y)/(noiseDim.y)) * flippedUV;  
	vec3 randomVec = texture(ssaoNoise, noiseUV).xyz * 2.0 - 1.0;
	
	// Create TBN matrix
	vec3 tangent = normalize(randomVec - normal * dot(randomVec, normal));
	vec3 bitangent = cross(tangent, normal);
	mat3 TBN = mat3(tangent, bitangent, normal);

	// Calculate occlusion value
	float occlusion = 0.0f;
	// remove banding
	const float bias = 0.025f;
	for(int i = 0; i < SSAO_KERNEL_SIZE; i++)
	{		
		vec3 samplePos = TBN * uboSSAOKernel.samples[i].xyz; 
		samplePos = fragPos + samplePos * SSAO_RADIUS; 
		
		// project
		vec4 offset = vec4(samplePos, 1.0f);
		offset = projection * offset; 
		offset.xyz /= offset.w; 
		offset.xyz = offset.xyz * 0.5f + 0.5f; 
		
		float res = texture(samplerPositionDepth, offset.xy).w;
		if(res == 0.0)
		{
			res = 1.0;
		}

		float sampleDepth = -res; 

		float rangeCheck = smoothstep(0.0f, 1.0f, SSAO_RADIUS / abs(fragPos.z - sampleDepth));
		occlusion += (sampleDepth >= samplePos.z + bias ? 1.0f : 0.0f) * rangeCheck;           
	}
	occlusion = 1.0 - (occlusion / float(SSAO_KERNEL_SIZE));
	
	outFragColor = occlusion;
}
