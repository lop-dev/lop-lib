---------------------------------------------------------------------------------
--      This code was created by League of Perfect (lop_dev@hotmail.com)
--      You can get the latest version from : github.com/lop-dev/lop-lib
---------------------------------------------------------------------------------

local SignalBus = require("LULib.Framework.Base.SignalBus")

--local BaseView = class("BaseView", import("..Base.Notifier"))
local BaseView = class("BaseView")

BaseView.PREFAB_PATH  = 'xxxx'              -- 每个界面对应的prefab路径
BaseView.gameObject   = nil                 -- 每个界面对应的prefab

BaseView._viewParams  = nil                 -- 界面参数
BaseView._isLoading   = false               -- 标识当前界面prefab是否在加载中
BaseView._hideFlag    = false               -- 仅用于界面prefab还没load完成就隐藏的界面
BaseView._releaseType = CReleaseType.Manual     -- AB释放类型，Normal自动释放，Manual手动释放
BaseView._destroyWidgetList  = nil              -- 用来存储手动释放的Widget,否则widget会被tolua中间层缓存导致泄漏

function BaseView:ctor()
    error("[BaseView]>> abstract class cannot be instantiated!")
end


------------------------------------------------------------------------------------------------------------
--
-- private function
--
------------------------------------------------------------------------------------------------------------

function BaseView:addContextListener(uniqueId, callback, context)
    SignalBus:getInstance():addContextListener(uniqueId, callback, context)
end

function BaseView:delContextListener(uniqueId, callback, context)
    SignalBus:getInstance():delContextListener(uniqueId, callback, context)
end

function BaseView:delContextListeners(listenerContext)
    SignalBus:getInstance():delContextListeners(listenerContext)
end

------------------------------------------------------------------------------------------------------------
--
-- public function
--
------------------------------------------------------------------------------------------------------------
function BaseView:dispatch(uniqueId, ...)
    SignalBus:getInstance():dispatch(uniqueId, ...)
end

-- @params isStrict 是否严格意义上的可见。默认true
function BaseView:isVisible(isStrict)
    isStrict = isStrict ~= false

    if isStrict then
        return not isNil(self.gameObject) and self.gameObject.activeInHierarchy
    end

    return not isNil(self.gameObject) and self.gameObject.activeSelf
end

--- 获取XxxService
-- @return 默认获取该模块相关的Service,此时可不填参数（但必须保证模块的前缀相同）。提供参数时，获取指定Service
-- @params sName Service名
function BaseView:getService(sName)
    if sName then 
        return ServiceManager:getService(sName) 
    end

    return ServiceManager:getService(string.match(self.type, "^%u?%l*") .. "System")
end

--- 获取XxxService(同上)
function BaseView:getModel(mName)
    if mName then 
        return ServiceManager:getModel(mName) 
    end

    return ServiceManager:getService(string.match(self.type, "^%u?%l*") .. "System"):getModel()
end

------------------------------------------------------------------------------------------------------------
--
-- protected function
--
------------------------------------------------------------------------------------------------------------

-- first show or update here
function BaseView:onShow(...)
    if IsLogDebug and ViewManager.VERBOSE_LEVEL >= 3 then
        print("[BaseView]>> call function 'onShow': " .. self.type)
    end
    self:dispatch(GameConst.ON_VIEW_SHOW, self.type)
end

-- exit here
function BaseView:onExit(isDestroy)
    if isDestroy == true then
         self:_ClearWidgets()

        -- 有界面销毁，释放资源的引用计数
--        warning("+++++++++++ secotion销毁，释放资源引用计数++++++++++++++++++" .. self.PREFAB_PATH)
        CResourcesManager.Instance:ReleaseAssetbundle(ERESOURCE_TYPE.UI_PREFABS, self.PREFAB_PATH)
    end

    --抛一个事件出来,用来判定其他界面是否属于当前打开的界面
    self:dispatch(GameConst.ON_VIEW_EXIT, self.type, isDestroy)
end

function BaseView:onInitialize()
    self:initLocalization()
end

function BaseView:addListeners()
    --self:addContextListener(GameConst.ON_REGION_CHANGE, self.updateLocalization, self)
end

function BaseView:delListeners()
    --self:delContextListener(GameConst.ON_REGION_CHANGE, self.updateLocalization, self)
end

function BaseView:setVisible(isVisible, isResetLoading)
    if IsLogDebug and ViewManager.VERBOSE_LEVEL >= 3 then
        print("[BaseView]>> call function 'setVisible': " .. tostring(isVisible) .. " panel: " .. self.type)
    end
    -- 验证界面是否有效
    if self:checkInvalid(isVisible, isResetLoading) then
        return false
    end

    --self:verifyGameobject(false)
    -- self.gameObject:SetActive(isVisible)
    if not isNil(self.gameObject) then
        CUtilObj2Lua.SetActive(self.gameObject, isVisible)
    else
        return false
    end

    return true
end

--- 仅setVisible方法内调用。本界面prefab是否加载完成就操作界面视为无效界面，需reopen
-- @params isResetLoading 是否重置self._isLoading标志。默认false
function BaseView:checkInvalid(isVisible, isResetLoading)
    isResetLoading = isResetLoading == true

    if self._hideFlag or self._isLoading then
        if not isVisible then
            self._hideFlag = true
            if self._isLoading and isResetLoading then self._isLoading = false end
        end
        return true
    end

    return false
end

--- 验证界面是否存在gameobject
-- @params showError 是否显示错误信息。默认true
function BaseView:verifyGameobject(showError)
    showError = showError ~= false and true or false

    local result = not isNil(self.gameObject)
    if showError and (not result) then
        if IsLogDebug then
            error("[BaseView]>> view panel error! " .. self.type)
        end
    end

    return result
end

function BaseView:loadPrefab(...)
    if IsLogDebug and ViewManager.VERBOSE_LEVEL >= 3 then
        print("[BaseView]>> load prefab: " .. self.type .. " isloading: " .. tostring(self._isLoading))
    end

    self._viewParams = {...}
    if self._isLoading then return end
    self._isLoading = true

    CommonUtil:getResourcePrefabClone(ERESOURCE_TYPE.UI_PREFABS, self.PREFAB_PATH, true,
        function(obj)
            if isNil(obj) then
                if IsLogDebug then
                    error("[BaseView]>> loadPrefab failed! " .. tostring(self.PREFAB_PATH))
                end
                self._isLoading = false
                CResourcesManager.Instance:ReleaseAssetbundle(ERESOURCE_TYPE.UI_PREFABS, self.PREFAB_PATH)
                return
            end

            if IsLogDebug then
                log(string.substitute("loadPrefab : {0} finish" , self.PREFAB_PATH), "[BaseView]>> ")
            end
            self._isLoading = false
            self.gameObject = obj
            self.gameObject.name = string.gsub(self.gameObject.name, "%b()", "") -- rename
            self:onLoadCompleteHandler(unpack(self._viewParams, 1, table.maxn(self._viewParams)))
        end, true, self._releaseType
    )
end

function BaseView:onLoadCompleteHandler(...)
    if IsLogDebug and ViewManager.VERBOSE_LEVEL >= 3 then
        print("[BaseView]>> call function 'onLoadCompleteHandler': " .. self.type)
    end
end

-- 多语言初始化
function BaseView:initLocalization()

end

-- 多语言刷新
function BaseView:updateLocalization()
    
end

-- 收集待销毁的组件,无视重复添加（tolua销毁
 function BaseView:collectWidgetForDestroy(widget)
     if IsLogDebug and not self._destroyWidgetList then
         error("[BaseView]>> property '_destroyWidgetList' can't be nil: " .. self.type)
         return
     end

     table.insert(self._destroyWidgetList, widget)
 end

------------------------------------------------------------------------------------------------------------
--
-- private function
--
------------------------------------------------------------------------------------------------------------

 function BaseView:_ClearWidgets()
     if self._destroyWidgetList == nil then
         return
     end

     for i, v in ipairs(self._destroyWidgetList) do
         if not isNil(v) then
             Object.Destroy(v)
         end
     end
     self._destroyWidgetList = {}
 end

return BaseView
