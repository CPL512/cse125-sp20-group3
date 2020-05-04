/*
 * Copyright (c) 2018-2020 The Forge Interactive Inc.
 * 
 * This file is part of The-Forge
 * (see https://github.com/ConfettiFX/The-Forge).
 * 
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 * 
 *   http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
*/

struct PsIn
{    
    float3 pos               : POSITION;
	float3 normal	         : NORMAL;
	float2 texCoord          : TEXCOORD0;
};

Texture2D DiffuseTexture : register(t0);
SamplerState DefaultSampler : register(s0);

float4 main(PsIn input) : SV_TARGET
{
	float nDotl =  saturate((dot(normalize(input.normal), float3(0, 1, 0)) + 1.0f) * 0.5f);
	float3 color = DiffuseTexture.Sample(DefaultSampler, input.texCoord).rgb;
    return float4(color * nDotl, 1.0f);
}