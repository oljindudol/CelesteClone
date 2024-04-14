#pragma once


//About Tile Map
#define STR_KEY_TILEFAP_FORE_GROUND L"TileFore"
#define STR_KEY_TILEFAP_BACK_GROUND L"TileBack"
#define STR_KEY_TILEFAP L"TileMap"

#define STR_KEY_TILEFAB_PATH L"\\tilefab\\"
#define STR_KEY_DREAMFAB_PATH L"\\dreamfab\\"



//TextureArr
#define STR_KEY_TEXARR_FORE_TILE L"TileMapForeGroundTextureArray"
#define STR_KEY_TEXARR_BACK_TILE L"TileMapBakcGroundTextureArray"
#define STR_KEY_TEXARR_ANIMATED_PARTICLE L"AniMatedParticleTextureArray"
#define STR_KEY_TEXARR_DREAMBLOCK_PARTICLE L"DreamBlockTextureArray"


// Image Path
#define STR_FILE_PATH_HAIR L"\\texture\\Gameplay\\hair\\hair00.png"
//#define STR_FILE_PATH_HAIR L"\\texture\\Effect1.png"
#define STR_FOLDER_PATH_BANGS L"\\texture\\Gameplay\\hair\\bangs"
#define STR_FOLDER_PATH_PLAYERANIMATION L"\\texture\\Gameplay\\madeline"
#define STR_FOLDER_PATH_FORE_TILE L"\\texture\\Gameplay\\tilemap\\foretile"
#define STR_FOLDER_PATH_BACK_TILE L"\\texture\\Gameplay\\tilemap\\backtile"
#define STR_FILE_PATH_PARTICLE_GENERAL L"\\texture\\Gameplay\\particle\\General\\particle1by1.png"
#define STR_FILE_PATH_RAY L"texture\\particle\\ray.png"

#define STR_FOLDER_PATH_PARTICLE_STARS L"\\texture\\Gameplay\\particle\\stars"
#define STR_FILE_PATH_PARTICLE_STARS L"\\texture\\Gameplay\\particle\\stars\\a.png"

#define STR_FOLDER_PATH_DREAMBLOCK L"\\texture\\Gameplay\\dreamblock"
#define STR_FILE_PATH_DREAMBLOCK_PARTICLE L"\\texture\\Gameplay\\dreamblock\\particles.png"

#define STR_FILE_PATH_TRANSITION L"\\texture\\Gameplay\\transition\\"
#define STR_FILE_TRANSITION_STAR L"STAR"
#define STAR_FILE_EXTENSION_PNG L".png"

// Imgui UI String


// ImGui ComponentUI String
#define STR_COMPONENT_UI_TileMapEditor "##TileMapEditor"



// TileInfoAtlasType
#define INT_TILE_INFO_ATLAS_TYPE_BACK_GROUND 0
#define INT_TILE_INFO_ATLAS_TYPE_FORE_GROUND 1

// Message



// Resource¿« key == path


// Default font



/////////////// fx Path ///////////////
#define STR_FILE_PATH_PlayerShader L"shader\\player.fx"
#define STR_FILE_PATH_Std2dShader L"shader\\std2d.fx"
#define STR_FILE_PATH_DebugShader L"shader\\debug.fx"
#define STR_FILE_PATH_ParticleShader L"shader\\particle.fx"
#define STR_FILE_PATH_PostProcessShader L"shader\\postprocess.fx"
#define STR_FILE_PATH_TileMapShader L"shader\\tilemap.fx"

#define STR_FILE_PATH_SetColorShader L"shader\\setcolor.fx"
#define STR_FILE_PATH_ParticleUpdateShader L"shader\\particle_update.fx"
#define STR_FILE_PATH_DreamParticleUpdateShader L"shader\\dreamparticle_update.fx"
#define STR_FILE_PATH_DreamParticleShader L"shader\\dreamparticle.fx"
#define STR_FILE_PATH_DrawLineShader L"shader\\drawline.fx"

#define STR_FILE_PATH_TransitionParticleUpdateShader L"shader\\transitionparticle_update.fx"
#define STR_FILE_PATH_TransitionParticleShader L"shader\\transitionparticle.fx"

// std2.fx functions name
#define STR_FUNC_NAME_VTXShaderHair "VS_Hair"
#define STR_FUNC_NAME_PIXShaderHair "PS_Hair"
#define STR_FUNC_NAME_VTXShaderStd2d "VS_Std2D"
#define STR_FUNC_NAME_PIXShaderStd2d "PS_Std2D"
#define STR_FUNC_NAME_VTXShaderGrayFilter "VS_GrayFilter"
#define STR_FUNC_NAME_PIXShaderGrayFilter "PS_GrayFilter"

// tilemap.fx functions name
#define STR_FUNC_NAME_VTXShaderTileMap "VS_TileMap"
#define STR_FUNC_NAME_PIXShaderTileMap "PS_TileMap"
#define STR_FUNC_NAME_VTXShaderGrid "VS_Grid"
#define STR_FUNC_NAME_PIXShaderGrid "PS_Grid"

// particle.fx functions name
#define STR_FUNC_NAME_VTXShaderParticle "VS_Particle"
#define STR_FUNC_NAME_GEOShaderParticle "GS_Particle"
#define STR_FUNC_NAME_PIXShaderParticle "PS_Particle"

#define STR_FUNC_NAME_GEOShaderParticleCustom "GS_CustomParticle"
//#define STR_FUNC_NAME_PIXShaderParticleCustom "PS_CustomParticle"

#define STR_FUNC_NAME_PIXShaderParticleAnimated "PS_AnimatedParticle"

// postprocess.fx functions name
#define STR_FUNC_NAME_VTXShaderDistorton "VS_Distortion"
#define STR_FUNC_NAME_PIXShaderDistorton "PS_Distortion"

// postprocess.fx functions name
#define STR_FUNC_NAME_VTXShaderDistorton "VS_ShockWave"
#define STR_FUNC_NAME_PIXShaderDistorton "PS_ShockWave"

// debug.fx functions name
#define STR_FUNC_NAME_VTXShaderDebugShape "VS_DebugShape"
#define STR_FUNC_NAME_PIXShaderDebugShape "PS_DebugShape"

// dreamparticle.fx
#define STR_FUNC_NAME_GEOShaderDreamParticle "GS_DreamParticle"
#define STR_FUNC_NAME_PIXShaderDreamParticle "PS_DreamParticle"

// computershader.fx functions name
#define STR_FUNC_NAME_CSShaderSetColor "CS_SetColor"
#define STR_FUNC_NAME_CSShaderParticleUpdate "CS_ParticleUpdate"
#define STR_FUNC_NAME_CSShaderDreamParticleUpdate "CS_DreamParticleUpdate"

#define STR_FUNC_NAME_VTXShaderDrawLine "VS_DrawLine"
#define STR_FUNC_NAME_GEOShaderDrawLine "GS_DrawLine"
#define STR_FUNC_NAME_PIXShaderDrawLine "PS_DrawLine"

#define STR_FUNC_NAME_CSShaderTransitionParticleUpdate "CS_TransitionParticleUpdate"

#define STR_FUNC_NAME_GEOShaderTransition "GS_TransitionParticle"
////////// Resource Key ///////////////
// Mesh
#define STR_KEY_PointMesh L"PointMesh"
#define STR_KEY_RectMesh L"RectMesh"
#define STR_KEY_RectMesh_Debug L"RectMesh_Debug"
#define STR_KEY_CircleMesh L"CircleMesh"
#define STR_KEY_CircleMesh_Debug L"CircleMesh_Debug"
#define STR_KEY_CrossMesh L"CrossMesh"



// compute shader
#define STR_KEY_SetColorShader L"SetColorShader"
#define STR_KEY_ParticleUpdateShader L"ParticleUpdateShader"
#define STR_KEY_DreamParticleUpdateShader L"DreamParticleUpdateShader"
#define STR_KEY_TransitionParticleUpdateShader L"TransitionParticleUpdateShader"



// Shader
#define STR_KEY_PlayerShader L"PlayerShader"
#define STR_KEY_Std2dShader L"Std2DShader"
#define STR_KEY_EffectShader L"EffectShader"
#define STR_KEY_TileMapShader L"TileMapShader"
#define STR_KEY_GridShader L"GridShader"
#define STR_KEY_ParticleRenderShader L"ParticleRenderShader"
#define STR_KEY_GrayFilterShader L"GrayFilterShader"
#define STR_KEY_DistortionShader L"DistortionShader"
#define STR_KEY_DebugShapeShader L"DebugShapeShader"
#define STR_KEY_HairShader L"PlayerHairShader"
#define STR_KEY_BangShader L"PlayerBangShader"
#define STR_KEY_ShockWaveShader L"ShockWaveShader"
#define STR_KEY_CustomParticleRenderShader L"CustomParticleRenderShader"
#define STR_KEY_AnimatedParticleRenderShader L"AnimatedParticleRenderShader"
#define STR_KEY_DreamParticleRenderShader L"DreamParticleRenderShader"
#define STR_KEY_DrawLineRenderShader L"DrawLineRenderShader"
#define STR_KEY_TransitionRenderShader L"TransitionRenderShader"


// Material
#define STR_KEY_Std2dMeterial L"Std2DMtrl"
#define STR_KEY_BackGroundMeterial L"BackgroundMtrl"
#define STR_KEY_TileMapMeterial L"TileMapMtrl"
#define STR_KEY_GridMeterial L"GridMtrl"
#define STR_KEY_ParticleMeterial L"ParticleMtrl"
#define STR_KEY_GrayFilterMeterial L"GrayFilterMtrl"
#define STR_KEY_DistortionMeterial L"DistortionMtrl"
#define STR_KEY_DebugMeterial L"DebugShapeMtrl"
#define STR_KEY_HairMeterial L"PlayerHairMtrl"
#define STR_KEY_BangMeterial L"PlayerBangMtrl"
#define STR_KEY_ShockWaveMeterial L"ShockWaveMtrl"
#define STR_KEY_CustomParticleMeterial L"CustomParticleMtrl"
#define STR_KEY_AnimatedParticleMeterial L"AnimatedParticleMtrl"
#define STR_KEY_DreamParticleMeterial L"DreamParticleMtrl"
#define STR_KEY_DrawLineMeterial L"DrawLineMtrl"
#define STR_KEY_TransitionParticleMeterial L"TransitionParticleMtrl"



// Resource View Key

// Tool Object name
//#define STR_TOOL_OBJ_NAME_ToolCamera _T("Tool Camera_Engine")
//#define STR_TOOL_OBJ_NAME_ToolUICamera _T("Tool UI Camera_Engine")
//#define STR_TOOL_OBJ_NAME_ToolOrthogonalAxisCoordinate _T("Tool OrthogonalAxisCoordinate")
//#define STR_TOOL_OBJ_NAAME_DragAndDrop _T("Tool Drag_And_Drop")


 /////// / // Test ///// / / /



// Editor Object Name
#define STR_Editor_OBJ_NAME_EditorCamera L"Editor Camera"