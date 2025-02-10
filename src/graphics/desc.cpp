#include "./desc.hpp"
#include "./callback.hpp"

WGPUDeviceDescriptor createDeviceDesc()
{
	WGPUDeviceDescriptor deviceDesc = {};
	
	deviceDesc.nextInChain = nullptr;
	deviceDesc.label = "My Device"; // anything works here, that's your call
	deviceDesc.requiredFeatureCount = 0; // we do not require any specific feature
	deviceDesc.requiredLimits = nullptr; // we do not require any specific limit
	deviceDesc.defaultQueue.nextInChain = nullptr;
	deviceDesc.defaultQueue.label = "The default queue";
	// {{Set device lost callback}}
	deviceDesc.deviceLostCallback = deviceLostCallback;
	// {{Set uncaptured error callback}}
	WGPUUncapturedErrorCallbackInfo callbackInfo = {};
	callbackInfo.nextInChain = nullptr;
	callbackInfo.callback = onDeviceError;
	deviceDesc.uncapturedErrorCallbackInfo = callbackInfo;
	
	return deviceDesc;
}

WGPUCommandEncoderDescriptor createEncoderDesc()
{
	WGPUCommandEncoderDescriptor encoderDesc = {};
	
	encoderDesc.nextInChain = nullptr;
	encoderDesc.label = "My command encoder";
	
	return encoderDesc;
}

WGPURenderPassDescriptor createRenderPassDesc(const WGPURenderPassColorAttachment& renderPassColorAttach)
{
	WGPURenderPassDescriptor renderPassDesc = {};
	
	renderPassDesc.nextInChain = nullptr;
	renderPassDesc.colorAttachmentCount = 1;
	renderPassDesc.colorAttachments = &renderPassColorAttach;
	renderPassDesc.depthStencilAttachment = nullptr;
	renderPassDesc.timestampWrites = nullptr;
	
	return renderPassDesc;
}
