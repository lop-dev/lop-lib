-- 为 Lua 层提供切换场景的调用接口
local SceneManager = class("SceneManager")

SceneManager.ON_SCENE_ENTER = "ON_SCENE_ENTER"                     -- 进入指定场景

function SceneManager:ctor()
    self._onSceneLoaded     = nil                  -- 用于存储加载回调
    self._curLoadingSceneId = -1                   -- 当前正在加载的场景id
    self._isLoading         = false                -- 是否正在切换场景
end

function SceneManager:getInstance()
    if self.instance == nil then
        self.instance = self:New()
    end

    return self.instance
end

--- 进入场景
function SceneManager:onReceiveReqEnterScene(sceneId)
    self._isLoading = true
    --- 先清理
    Framework.ViewPool:clearPool()
    ViewManager:closeAllPanel()

    --- 开始加载场景
    CWorldDisplay.Instance:LoadScene(sceneId)
    -- 注册update函数
    LuaTimer.RegisterMultiUpdate(self._LoadingSceneUpdate, self)
end

--- 是否正在加载场景
function SceneManager:isLoading()
    return self._isLoading
end

--- 切换场景
function SceneManager:loadScene(sceneId, callback)
    if IsLogDebug then
        print("[SceneManager]>> change scene from: " .. tostring(self._curLoadingSceneId) .. " to " .. sceneId)
    end
    self._onSceneLoaded = callback
    self:onReceiveReqEnterScene(sceneId)
end

-------------------------------------------------------------------------------------------------------------
-- private function
-------------------------------------------------------------------------------------------------------------

function SceneManager:_LoadingSceneUpdate()
    local progress = CWorldDisplay.Instance:Progress()
    if progress >= 1.0 then
        self:_LoadingComplete()
    else
        Framework.SignalBus:dispatch(GameConst.SET_LOADING_PROGRESS, progress)
    end
end

function SceneManager:_LoadingComplete()
    if IsLogDebug then
        log("[SceneManager]>> LoadingComplete")
    end

    self._isLoading = false
    LuaTimer.UnregisterMultiUpdate(self._LoadingSceneUpdate, self)

    self:_DoLoadingComplete()
end

--加载场景完成
function SceneManager:_DoLoadingComplete()
    self:_PlaySound()
    
end

function SceneManager:_PlaySound()
    if self._sceneConfig == nil then
        return
    end
end

--- 获取当前场景id
function SceneManager:getCurSceneId()
    return self._curLoadingSceneId
end
return SceneManager
