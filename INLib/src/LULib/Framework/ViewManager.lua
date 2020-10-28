---------------------------------------------------------------------------------
--      This code was created by League of Perfect (lop_dev@hotmail.com)
--      You can get the latest version from : github.com/lop-dev/lop-lib
---------------------------------------------------------------------------------
local ViewManager              = {}
ViewManager.UNIQUE_ID          = 10000 --界面唯一id编号(container)
ViewManager.MAX_CONTAINER      = 5 --同时存在container最大数量
--- 界面事件
ViewManager.ON_VIEW_ENTER      = "ON_VIEW_ENTER"  -- 打开指定界面
ViewManager.ON_VIEW_EXIT       = "ON_VIEW_EXIT"   -- 退出指定界面

ViewManager._cachedContainers  = {}       --container缓存
ViewManager._openingContainers = {}       --当前打开的container列表
-------------------------------------------------------------------------------------------------------------
-- public function
-------------------------------------------------------------------------------------------------------------
-- 显示界面
function ViewManager:showPanel(className, ...)
    local viewClass    = GameUIPath:getClass(className)
    if self:isContainer(viewClass) then
        local tContainer = self:getRunningContainer()
        if tContainer and tContainer.type == viewClass.type and tContainer:isActive() then
            print("container is opening!")
            return
        end
        self:checkOverflowContainer()
        local container = self:getCachedContainer(viewClass)
        if container == nil then
            container = self:createContainer(viewClass)
        end
        container:setRootObjActive(true)
        container:doShow(...)
    elseif self:isPanel(viewClass) then
        local curContainer = self:getRunningContainer()
        if not curContainer then
            if IsLogDebug then
                error("Error: current container is nil")
            end
            return
        end

        local tInstance = curContainer:getCachedViewItem(viewClass)
        if tInstance == nil then
            tInstance = curContainer:creatViewItem(viewClass)
        end
        tInstance.mContainer = curContainer
        tInstance:doShow(...)
    else
        if IsLogDebug then
            error("usage error!")
        end
    end
end

-- 按类名退出界面，（container有重名界面从后往前只退出靠后一个）
-- @params className 类名
-- @params isShowPre 上否显示上一个界面。目前仅用于容器界面
function ViewManager:exitPanel(className, isShowPre)
    local tClass       = GameUIPath:getClass(className)
    local tInstance    = nil

    if self:isContainer(tClass) then
        tInstance = self:getOpeningContainerByType(tClass)
    elseif self:isPanel(tClass) then
        local tContainer = self:getRunningContainer()
        if not tContainer then
            if IsLogDebug then
                error("current running container is nil")
            end
            return
        end
        tInstance = tContainer:getCachedViewItem(tClass)
    else
        if IsLogDebug then
            error("usage error!")
        end
    end
    if tInstance then
        tInstance:doExit(false, isShowPre)
        if tInstance:getDestroyImmediately() then
            self:destroyPanel(tInstance)
        end
    end
    self:printContainersLog("exitPanel")
end

-- 按类退出界面
-- @params viewClass 类实例
-- @params isShowPre 上否显示上一个界面。目前仅用于容器界面
function ViewManager:exitPanelByClass(viewClass, isShowPre)
    local tInstance = nil
    if self:isContainer(viewClass) or self:isPanel(viewClass) then
        tInstance = viewClass
    else
        if IsLogDebug then
            error("usage error!")
        end
    end
    if tInstance then
        tInstance:doExit(false, isShowPre)
        if tInstance:getDestroyImmediately() then
            self:destroyPanel(tInstance)
        end
    end
end
--检测container数量限制
function ViewManager:checkOverflowContainer()
    local num = #self._openingContainers
    if self.MAX_CONTAINER > 0 and  num >= self.MAX_CONTAINER then
        for i, item in ipairs(self._openingContainers) do
            if item and (not item.DontDestroyOnLoad) then
                self:destroyPanel(item)
                if IsLogDebug then
                    print("container be force destroyed by overflow check! : " .. item.type .. item.uniqueID)
                end
                return
            end
        end
    end
end

-- 关闭所有界面
--@params  isDestroy 是否Destory, 默认false
function ViewManager:closeAllPanel(isDestroy)
    if isDestroy == nil then
        isDestroy = false
    end
    self:printContainersLog("closeAllPanel before")
    
    for _, item in ipairs(self._cachedContainers) do
        if item then
            if item.DontDestroyOnLoad then
                item:doExit(false)
            else
                if isDestroy then
                    self:destroyPanel(item)
                else
                    item:doExit(false)
                end
            end
        end
    end

    self:printContainersLog("closeAllPanel after")
    -- 有界面销毁，执行GC, 看看会不会引起卡顿
    --[[
    if IsLogDebug then
        self:print("场景切换，执行GC")
    end
    LuaGC()
    --]]
end

-------------------------------------------------------------------------------------------------------------
--
-- @internal
--
-------------------------------------------------------------------------------------------------------------
--- 获取当前显示的模块容器
function ViewManager:getRunningContainer()
    local tLength = #self._openingContainers
    if tLength > 0 then
        return self._openingContainers[tLength]
    end
    return nil
end
-------------------------------------------------------------------------------------------------------------
--
-- @private
--
-------------------------------------------------------------------------------------------------------------

--- 销毁界面
function ViewManager:destroyPanel(viewInstance)
    --主界面永远不销毁
    if viewInstance.DontDestroyOnLoad then
        return
    end
    if IsLogDebug then
        self:print("destroyPanel : " .. viewInstance.type)
    end
    local bIsContainer = self:isContainer(viewInstance)
    if bIsContainer then
        self:removeOpeningContainer(viewInstance)
        self:removeCachedContainer(viewInstance)
        viewInstance:doExit(true)
    else
        if viewInstance.mContainer then
            viewInstance.mContainer:removeOpeningViewItem(viewInstance)
            local tPanel = viewInstance.mContainer:removeCachedViewItem(viewInstance)
            if tPanel then
                tPanel:doExit(true)
            end
        end
    end
    -- 有界面销毁，执行GC, 看看会不会引起卡顿
    CResourcesManager.Instance:DoGC()
end

-------------------------------------------------------------------------------------------------------------
-- 容器相关
-------------------------------------------------------------------------------------------------------------
--开启新Container，隐藏前面已开启Container
function ViewManager:hidePreOpeningContainer(container)
    if container then
        local index = #self._openingContainers
        if index > 0 and self._openingContainers[index] == container then
            index = index - 1
            if index > 0 then
                for i = 1, index do
                    self._openingContainers[i]:setRootObjActive(false)
                end
            end
        end
    end
end
--开启前面已隐藏的Container
function ViewManager:showPreOpeningContainer(container)
    local index = #self._openingContainers
    if index > 0 and self._openingContainers[index] ~= container then
        self._openingContainers[index]:setRootObjActive(true)
    end
end
--获取缓存Container
function ViewManager:getCachedContainer(container)
    if self._cachedContainers and container then
        for i, item in ipairs(self._cachedContainers) do
            if item.type == container.type then
                --跳过已经打开的界面
                if self:getOpeningContainer(item) == nil then
                    return item
                end
            end
        end
    end
    return nil
end
--移除缓存
function ViewManager:removeCachedContainer(container)
    if self._cachedContainers and container then
        for i, item in ipairs(self._cachedContainers) do
            if item:Equal(container) then
                table.remove(self._cachedContainers, i)
                return item
            end
        end
    end
    return nil
end
function ViewManager:createContainer(container)
    local tInstance = container.New()
    table.insert(self._cachedContainers, tInstance)

    return tInstance
end
--获取已开启界面
function ViewManager:getOpeningContainer(container)
    if container and self._openingContainers then
        for i, item in ipairs(self._openingContainers) do
            if item:Equal(container) then
                return item
            end
        end
    end
    return nil
end
--在已打开列表按类名查找
function ViewManager:getOpeningContainerByType(container)
    if container and self._openingContainers then
        local num = #self._openingContainers
        for i = num, 1, -1 do
            if self._openingContainers[i].type == container.type then
                return self._openingContainers[i]
            end
        end
    end
    return nil
end
--将界面移动到开启队列的末端
function ViewManager:updateOpeningContainers(container)
    if container and self._openingContainers then
        local num = #self._openingContainers
        for i = num, 1, -1 do
            if self._openingContainers[i]:Equal(container) then
                table.remove(self._openingContainers, i)
            end
        end
    end
    table.insert(self._openingContainers, container)
end

--移除已开启container,添加到缓存内
function ViewManager:removeOpeningContainer(container)
    if container and self._openingContainers then
        for i, item in ipairs(self._openingContainers) do
            if item:Equal(container) then
                table.remove(self._openingContainers, i)
                break
            end
        end
    end
end

--判断是否Container
function ViewManager:isContainer(viewClass)
    return viewClass._baseViewType and viewClass._baseViewType == 3
end
--判断是否Panel
function ViewManager:isPanel(viewClass)
    return viewClass._baseViewType and viewClass._baseViewType == 2
end
--判断是否为section
function ViewManager:isSection(viewClass)
    return viewClass._baseViewType and viewClass._baseViewType == 1
end

function ViewManager:getUniqueID()
    self.UNIQUE_ID = self.UNIQUE_ID + 1
    return self.UNIQUE_ID
end
-- 输出容器类log
-- @params isOpened 是否当前正打开的界面。默认true
function ViewManager:printContainersLog(title)
    if not IsLogDebug then
        return
    end
    title = title and title or ""
    self:print(title .. "--------------------log start---------------")
    local tmp = {}
    table.insert(tmp, "Cache Containers: ")
    for _, val in ipairs(self._cachedContainers) do
        table.insert(tmp, tostring(val))
    end
    self:print(table.concat(tmp, "\n"))
    
    tmp = {}
    table.insert(tmp, "Opening Containers: ")
    for _, val in ipairs(self._openingContainers) do
        table.insert(tmp, tostring(val))
    end
    self:print(table.concat(tmp, "\n"))
    self:print("--------------------log end---------------")
end
-------------------------------------------------------------------------------------------------------------
-- 界面销毁相关
-------------------------------------------------------------------------------------------------------------
--log输出
function ViewManager:print(str)
    if IsLogDebug then
        log(str, "[ViewManager]>> ")
    end
end

return ViewManager
