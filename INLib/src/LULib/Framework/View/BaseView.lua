---------------------------------------------------------------------------------
--      This code was created by League of Perfect (lop_dev@hotmail.com)
--      You can get the latest version from : github.com/lop-dev/lop-lib
---------------------------------------------------------------------------------

local SignalBus = require("LULib.Framework.Base.SignalBus")
local BaseView = class("BaseView")
--界面类型
BaseView._baseViewType = 0

BaseView.PREFAB_PATH        = 'xxxx'              -- 每个界面对应的prefab路径
BaseView.gameObject         = nil                 -- 每个界面对应的prefab
BaseView._viewParams        = nil                 -- 界面参数
BaseView._isLoading         = false               -- 标识当前界面prefab是否在加载中
BaseView._hideFlag          = false               -- 仅用于界面prefab还没load完成就隐藏的界面
BaseView._destroyed         = false               -- 标志界面是否已销毁
BaseView._releaseType       = CReleaseType.Manual -- AB释放类型，Normal自动释放，Manual手动释放
BaseView._destroyWidgetList = nil                 -- 用来存储手动释放的Widget,否则widget会被tolua中间层缓存导致泄漏

function BaseView:ctor()
    error("[BaseView]>> subclass must override this function!")
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

--判断ui是否显示激活
-- @params inHierarchy 是否严格意义上的可见
function BaseView:isActive(inHierarchy)
    if inHierarchy then
        return self:isGameObjectValid() and self.gameObject.activeInHierarchy
    end

    return self:isGameObjectValid() and self.gameObject.activeSelf
end
------------------------------------------------------------------------------------------------------------
--
-- protected function
--
------------------------------------------------------------------------------------------------------------

-- first show or update here
function BaseView:onShow(...)
    self:dispatch(GameConst.ON_VIEW_SHOW, self.type)
end

-- exit here
function BaseView:onExit(isDestroy)
    if isDestroy == true then
         self:_ClearWidgets()

        --有界面销毁，释放资源的引用计数
        --warning("+++++++++++ secotion销毁，释放资源引用计数++++++++++++++++++" .. self.PREFAB_PATH)
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

function BaseView:setVisible(isVisible)
    if IsLogDebug then
        error("[BaseView]>> subclass must override this function!")
    end
end

--判断obj是否有效
function BaseView:isGameObjectValid()
    if self.gameObject and (not isNil(self.gameObject)) then
        return true
    end

    return false
end

--判断当前界面是否有效
function BaseView:isValid()
    if self:getHideFlag() or self:getIsLoading() then
        return false
    end
    return true
end

--隐藏标记操作
function BaseView:getHideFlag()
    return self._hideFlag
end
function BaseView:setHideFlag( bValue )
    self._hideFlag = bValue
end
--加载状态操作
function BaseView:getIsLoading()
    return self._isLoading
end
function BaseView:setIsLoading( bValue )
    self._isLoading = bValue
end
--界面是否销毁操作
function BaseView:getDestroyed()
    return self._destroyed
end
function BaseView:setDestroyed(bValue)
    self._destroyed = bValue
end

function BaseView:destroyGameObject()
    if IsLogDebug then
        log("Destroy gameObject : " .. self.type, "[BaseView]>>")
    end
    GameObject.Destroy(self.gameObject)
    self.gameObject = nil
    self:setDestroyed(true)
end
--加载prefab
function BaseView:loadPrefab(...)
    if IsLogDebug then
        print("[BaseView]>> loadPrefab = " .. self.type .. " isloading = " .. tostring(self:getIsLoading()))
    end

    self._viewParams = {...}
    if self:getIsLoading() then
        return
    end
    self:setIsLoading(true)

    CommonUtil:getResourcePrefabClone(ERESOURCE_TYPE.UI_PREFABS, self.PREFAB_PATH, true,
        function(obj)
            self:setIsLoading(false)
            if isNil(obj) then
                if IsLogDebug then
                    error("[BaseView]>> loadPrefab failed! " .. tostring(self.PREFAB_PATH))
                end
                CResourcesManager.Instance:ReleaseAssetbundle(ERESOURCE_TYPE.UI_PREFABS, self.PREFAB_PATH)
                return
            end

            if IsLogDebug then
                log(string.substitute("loadPrefab success! {0} " , self.PREFAB_PATH), "[BaseView]>> ")
            end
            self.gameObject = obj
            self.gameObject.name = string.gsub(self.gameObject.name, "%b()", "")
            self:onLoadCompleteHandler(unpack(self._viewParams, 1, table.maxn(self._viewParams)))
        end,
        true,
        self._releaseType)
end

function BaseView:onLoadCompleteHandler(...)
    if IsLogDebug then
        print("[BaseView]>> onLoadCompleteHandler : " .. self.type)
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
    if self._destroyWidgetList and widget then
        table.insert(self._destroyWidgetList, widget)
    else
        if IsLogDebug then
            error("[BaseView]>> _destroyWidgetList is nil!")
        end
    end
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

     for _, v in ipairs(self._destroyWidgetList) do
         if not isNil(v) then
             Object.Destroy(v)
         end
     end
     self._destroyWidgetList = {}
 end

return BaseView
