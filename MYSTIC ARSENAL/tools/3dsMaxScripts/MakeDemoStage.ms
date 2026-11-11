/*!
 * MYSTIC ARSENAL デモ用 簡易ステージ生成スクリプト
 *
 * 使い方: 3ds Max の「スクリプト」→「スクリプトを実行」でこのファイルを開く。
 *   1. シーンをリセットしてステージ(床・外壁・柱・遮蔽物・高台+スロープ)を作る
 *   2. 各オブジェクトに k2EngineShader.fx の DirectX シェーダーマテリアルを割り当てる
 *   3. tkExporter と同じ処理で tkm を書き出す(保存ダイアログが出るので
 *      Game\Assets\modelData\demoStage\demoStage.tkm を指定する)
 *
 * 単位: 1 = 1cm。Z が上(3ds Max の座標系のまま)。
 * 原点付近はプレイヤーの初期位置、Y=+800 付近は敵の初期位置なので空けてある。
 */

-------------------------------------------------------------
-- パス設定(このスクリプトは tools\3dsMaxScripts に置く前提)
-------------------------------------------------------------
g_scriptDir   = getFilenamePath (getThisScriptFilename())               -- ...\tools\3dsMaxScripts\
g_toolsDir    = pathConfig.removePathLeaf g_scriptDir                   -- ...\tools
g_projectDir  = pathConfig.removePathLeaf g_toolsDir                    -- ...\MYSTIC ARSENAL
g_fxPath      = g_toolsDir + "\\3dsMaxShader\\k2EngineShader.fx"
g_srcTexDir   = g_projectDir + "\\Game\\Assets\\modelData\\stage\\"
g_outDir      = g_projectDir + "\\Game\\Assets\\modelData\\demoStage\\"

-- tkExporter のサブスクリプトを読み込む(tkExporter.ms と同じもの)
fileIn (g_scriptDir + "sub\\tkExporterCommon.ms")
fileIn (g_scriptDir + "sub\\tksExporter.ms")
fileIn (g_scriptDir + "sub\\tkmExporter.ms")

-------------------------------------------------------------
-- テクスチャを出力先フォルダにコピーする
-- (エンジンは tkm と同じフォルダの .dds を読むため)
-------------------------------------------------------------
fn CopyTexture texName =
(
	local src = g_srcTexDir + texName
	local dst = g_outDir + texName
	if doesFileExist dst do deleteFile dst
	copyFile src dst
	dst
)

-------------------------------------------------------------
-- k2EngineShader のマテリアルを作る
-------------------------------------------------------------
fn MakeK2Material matName texName =
(
	local texPath = CopyTexture texName
	local m = DxMaterial()
	m.name = matName
	m.effectfile = g_fxPath
	-- テクスチャを設定(エクスポーターは .fileName を読む)
	try (
		m.g_albedo = openBitMap texPath
	) catch (
		m.g_albedo = bitmapTexture filename:texPath
	)
	try ( m.g_useNormalMap = false ) catch()
	try ( m.g_useMetallicSmoothMap = false ) catch()
	m
)

-------------------------------------------------------------
-- 仕上げ:UV を付けてメッシュ化し、面ごとに頂点を分ける
-- (エクスポーターは頂点法線を書き出すので、角の頂点を分けないと
--  箱の陰影が丸くなってしまう)
-------------------------------------------------------------
fn FinishObject obj mat tileSize =
(
	addModifier obj (Uvwmap maptype:4 length:tileSize width:tileSize height:tileSize)
	convertToMesh obj
	meshop.explodeAllFaces obj 30.0
	update obj
	obj.material = mat
	obj
)

fn MakeBox objName pos w l h mat tileSize =
(
	-- pos は箱の底面の中心
	local b = box name:objName width:w length:l height:h pos:pos mapcoords:true
	FinishObject b mat tileSize
)

-------------------------------------------------------------
-- ステージを作る
-------------------------------------------------------------
fn BuildDemoStage =
(
	resetMaxFile #noPrompt
	makeDir g_outDir all:true

	local matFloor  = MakeK2Material "M_Floor"  "tiles_floor007.DDS"
	local matWall   = MakeK2Material "M_Wall"   "bricks007.DDS"
	local matPillar = MakeK2Material "M_Pillar" "tiles_wall001.DDS"
	local matCover  = MakeK2Material "M_Cover"  "gray.DDS"
	local matStep   = MakeK2Material "M_Step"   "Green.DDS"

	-- 床(40m 四方、上面が Z=0)
	MakeBox "Floor" [0,0,-20] 4000 4000 20 matFloor 400

	-- 外壁(高さ 4m、厚さ 50cm)
	MakeBox "Wall_N" [0, 2025,0] 4100 50 400 matWall 200
	MakeBox "Wall_S" [0,-2025,0] 4100 50 400 matWall 200
	MakeBox "Wall_E" [ 2025,0,0] 50 4000 400 matWall 200
	MakeBox "Wall_W" [-2025,0,0] 50 4000 400 matWall 200

	-- 柱(1m 四方、高さ 4m)
	MakeBox "Pillar_01" [-1200, 1200,0] 100 100 400 matPillar 200
	MakeBox "Pillar_02" [-1200,-1200,0] 100 100 400 matPillar 200
	MakeBox "Pillar_03" [ 1200,-1200,0] 100 100 400 matPillar 200
	MakeBox "Pillar_04" [-1700,    0,0] 100 100 400 matPillar 200

	-- 腰の高さの遮蔽物(敵の正面の通り道は空けておく)
	MakeBox "Cover_01" [ 800, 400,0]  60 300 120 matCover 200
	MakeBox "Cover_02" [-800,-400,0]  60 300 120 matCover 200
	MakeBox "Cover_03" [   0,-1300,0] 300 60 120 matCover 200

	-- 箱(1.5m 立方)
	MakeBox "Crate_01" [ 600,-1600,0] 150 150 150 matCover 150
	MakeBox "Crate_02" [-1500, 600,0] 150 150 150 matCover 150

	-- 高台(5m 四方、高さ 1.5m)
	MakeBox "Platform" [1400,1400,0] 500 500 150 matPillar 200

	-- 高台へのスロープ(X=550 で高さ 0、X=1150 で高さ 150)
	local verts = #([550,1250,0],[1150,1250,0],[1150,1550,0],[550,1550,0],[1150,1250,150],[1150,1550,150])
	local faces = #([1,4,3],[1,3,2],[1,5,6],[1,6,4],[2,3,6],[2,6,5],[1,2,5],[4,6,3])
	local ramp = mesh name:"Ramp" vertices:verts faces:faces
	FinishObject ramp matStep 200

	clearSelection()
	redrawViews()
)

-------------------------------------------------------------
-- 実行
-------------------------------------------------------------
BuildDemoStage()

-- tkExporter の「Save(*.tkm)」と同じ処理で書き出す
messageBox ("ステージを作成しました。\n続けて保存ダイアログで次の場所に保存してください。\n" + g_outDir + "demoStage.tkm") title:"MakeDemoStage"
exporter = TkmExporterForScript()
exporter.SaveTkm objects "" false
