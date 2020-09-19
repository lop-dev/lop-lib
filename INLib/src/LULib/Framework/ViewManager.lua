---------------------------------------------------------------------------------
--      This code was created by League of Perfect (lop_dev@hotmail.com)
--      You can get the latest version from : github.com/lop-dev/lop-lib
---------------------------------------------------------------------------------

local ViewManager = {}

--- 界面事件
ViewManager.ON_VIEW_ENTER = "ON_VIEW_ENTER"                     -- 打开指定界面
ViewManager.ON_VIEW_EXIT  = "ON_VIEW_EXIT"                      -- 退出指定界面
ViewManager.ON_VIEW_ENTER_FOR_GUIDE = "ON_VIEW_ENTER_FOR_GUIDE"                     -- 打开指定界面
ViewManager.ON_VIEW_EXIT_FOR_GUIDE  = "ON_VIEW_EXIT_FOR_GUIDE"                      -- 退出指定界面

-- 界面动效，目前只有弹出，如果后期有需求再扩充类型
ViewManager.VIEW_ANIM_NONE        = 0                           -- 无动效，默认情况
ViewManager.VIEW_ANIM_POPUP       = 1                           -- 弹出动效， 一般用于弹窗
ViewManager.VIEW_ANIM_POPUP_SCALE = 2                           -- 弹出动效， 一般用于弹窗(加缓动效果)
ViewManager.VIEW_ANIM_MOVE        = 10

--- 日志级别。0、关闭；1、界面打开、关闭信息；2、界面缓存信息；3、界面生命周期信息
ViewManager.VERBOSE_LEVEL = 3                             -- 是否显示界面管理相关Log

ViewManager._cachedContainers = {}                        -- 显示容器的缓存
ViewManager._openedContainerStack = {}                    -- 当前打开的容器的堆栈
ViewManager._cacheAlternative = {}                        -- 已打开的互斥界面
ViewManager._curAlternative = nil                         -- 当前打开的互斥界面

ViewManager.ALTERNATIVE_POPS = {                          -- 互斥pop界面
    xxxPaenl = 1,
    xxxPanel = 2,
    xxxpanel = 3,
    -- PetDtCommentPanel = 1,
    -- PetDtRenamePanel = 2,
    --ItemGotVersion3 = 4,
    --RelationLvIncreasePanel = 5,
}

--- 界面销毁相关
ViewManager.PANEL_CACHE_LENGTH = 10                              -- 待销毁界面缓存最大个数
ViewManager._dsCached          = {}                             -- 待销毁界面缓存
ViewManager._dsFlagStart       = 1                              -- 界面销毁权重
-------------------------------------------------------------------------------------------------------------
--
-- public function
--
-------------------------------------------------------------------------------------------------------------

--- 获取当前最上层打开的界面
-- @params filterPop 是否排除pop界面。默认false
function ViewManager:getCurShownPanel(filterPop)
    filterPop = filterPop == true and true or false

    local tContainer = self:getRunningContainer()
    if not tContainer then 
        return 
    end

    if filterPop then
        return tContainer:getTopFullView()
    end

    return tContainer:getTopView()
end

--- 关闭当前所有pop界面
-- @params isClose 是否关闭。当值为false时，仅隐藏界面（默认true
-- @params isClean 是否清理（默认false
function ViewManager:closeAllPopPanel(isClose, isClean)
    isClose = isClose ~= false
    isClean = isClean == true

    local container = self:getRunningContainer()
    if container then
        self:getRunningContainer():closeAllPopView(isClose, isClean)
    else
        self:print("current container is nil")
    end
end

--- 退出最上层界面
-- @params isContainer 是否为容器。当为容器时退出整个container。默认false
-- @params step 仅对 isContainer等于true时有用。表示回退几层container界面。目前仅支持0、-1、1，默认1
--              值为0时，退到MainUIContainer；
--              值为-1时，退到MainUIContainer但清理MainUIContainer；
--              值为1时，回退一层；
function ViewManager:popPanel(isContainer, step)
    isContainer = isContainer == true and true or false
    step = step or 1

    if isContainer then
        local tLength = #self._openedContainerStack
        if tLength < 1 then 
            return 
        end
        if step == 1 then
            if tLength <= 1 then 
                return 
            end
            self:exitPanel(GameUIPath[self._openedContainerStack[tLength].type], false)
        else
            local tIndex, tContainer = self._openedContainerStack[1].type == "MainUIContainer" and 2 or 1
            while tLength >= tIndex do
                tContainer = self._openedContainerStack[tIndex]
                self:exitPanel(GameUIPath[tContainer.type], false)
                tContainer:resetAllOpenedView()
                tLength = tLength - 1
            end
            if tIndex == 2 and step == -1 then 
                self._openedContainerStack[1]:resetAllOpenedView() 
            end    -- 清理MainUIContainer
        end
    else
        self:exitPanel(GameUIPath[self:getCurShownPanel().type])
    end
end

--- 显示打开过的最上层container
function ViewManager:showTopContainer(className, ...)
    local tLength = #self._openedContainerStack
    if tLength > 0 then -- 重新打开最上层容器界面
        local tInstance = self._openedContainerStack[tLength]
        -- 已打关闭标识或忽略不用重新打开的container。延时关闭
        local tFlagIgnoreReopen = tInstance:checkIsIgnoreReopen()
        if tFlagIgnoreReopen or tInstance:getExitFlag() then
            if IsLogDebug and self.VERBOSE_LEVEL >= 1 and tFlagIgnoreReopen then 
                self:print("ignore reopen container :" .. tInstance.type) 
            end
            self:exitPanel(GameUIPath[tInstance.type], false)
            self:showTopContainer(className, ...)
            return
        end
        -- 正常逻辑
        if IsLogDebug and self.VERBOSE_LEVEL >= 1 then 
            self:print("reopen container :" .. tInstance.type) 
        end
        if tInstance.type == "MainUIContainer" then                                                               -- 主界面特殊处理
            self:showPanel(GameUIPath.MainUIContainer)
        else
            -- local _, _, tName = string.find(className, "%.([%w_]+)$")
            -- tInstance:reopen(tName == tInstance.type and ... or nil)
            tInstance:reopen()
            self:printContainersLog()
        end

        -- 销毁操作
        local tPanelHit, tPanel = self:cachePanel(tInstance)
        if not tPanelHit then 
            return 
        end -- 未发生销毁
        self:_DestroyPanel(tPanelHit)
    else
        self:showPanel(className, ...)
    end
end

-- 替换container界面
function ViewManager:replaceContainer(className, ...)
    local tClass = GameUIPath:getClass(className)
    local tIsContainer, tInstance = isKindOf(tClass, "BaseContainer")
    if IsLogDebug and self.VERBOSE_LEVEL >= 1 then 
        self:print("show panel :" .. className) 
    end

    -- 显示界面
    if tIsContainer then
        tInstance = self:_PushContainer(tClass)
        if tInstance then 
            tInstance:doShow(true, ...)
        end
        self:printContainersLog()
    else
        if IsLogDebug and self.VERBOSE_LEVEL >= 1 then 
            self:print("it is not a 'BaseContainer' panel :" .. tClass.type) 
        end
    end
end

-- 预显示界面。解决同时打开多个界面闪的问题
function ViewManager:preShowPanel(className, ...)
    local tClass = GameUIPath:getClass(className)
    local tIsContainer, tInstance = isKindOf(tClass, "BaseContainer")
    if IsLogDebug and self.VERBOSE_LEVEL >= 1 then 
        self:print("pre show panel :" .. className) 
    end

    -- 显示界面
    if tIsContainer then
        tInstance = self:_PushContainer(tClass)
        if not tInstance then 
            return 
        end
        tInstance:doShow(false, ...)
        tInstance:markPreShowFlag()
    else
        local tContainer = self:getRunningContainer()
        if not tContainer then
            if IsLogDebug then
                error("show panel error!no 'BaseContainer' shown")
            end
            return
        end

        tInstance = tContainer:getViewItem(tClass) or tContainer:creatViewItem(tClass)
        tInstance:doShow(...)
    end
    tInstance:setVisible(false)
end

function ViewManager:concatViewInfo(isOpened)
    local tStr, tArr = "", (isOpened and self._openedContainerStack or self._cachedContainers)
    for i, item in ipairs(tArr) do
        tStr = (i > 1 and tStr ..  ", " or tStr) .. item.type
    end
    local tStrPrefix = isOpened and "opened" or "opening"
    return string.substitute("{0}:[{1}]", {tStrPrefix, tStr})
end

-- 显示界面
function ViewManager:showPanel(className, ...)
    local tClass = GameUIPath:getClass(className)
    local tIsContainer, tInstance = isKindOf(tClass, "BaseContainer")
    if IsLogDebug and self.VERBOSE_LEVEL >= 1 then 
        self:print("ViewManager:showPanel :" .. className)
    end
    -- 显示界面
    if tIsContainer then
        tInstance = self:_PushContainer(tClass)
        if tInstance then
            tInstance:doShow(false, ...)
        end
        self:printContainersLog()
    else
        local tContainer = self:getRunningContainer()
        if not tContainer then
            if IsLogDebug then
                error("show panel error!no 'BaseContainer' shown")
            end
            return
        end

        tInstance = tContainer:getViewItem(tClass) or tContainer:creatViewItem(tClass)

        local tPanel, tData = self:_CheckAlternative(tInstance, true, ...)
        if tPanel then -- 互斥处理
            if self._curAlternative == tPanel then 
                return 
            end

            if self._curAlternative then 
                self._curAlternative:setVisible(false) 
            end
            tPanel:doShow(unpack(tData))
            self._curAlternative = tPanel
        else
            tInstance:doShow(...)
        end
    end

    -- 销毁操作;缓存界面数量达到PANEL_CACHE_LENGTH，筛选可以销毁界面执行销毁操作
    if tInstance then
        local tPanelHit, tPanel = self:cachePanel(tInstance)
        if not tPanelHit then 
            return 
        end -- 未发生销毁
        self:_DestroyPanel(tPanelHit)
    end
end

-- 退出界面
-- @params isShowPre 上否显示上一个界面。目前仅用于容器界面
function ViewManager:exitPanel(className, isShowPre)
    if IsLogDebug and self.VERBOSE_LEVEL >= 1 then
        self:print("exit panel :" .. className)
    end
    local tClass = GameUIPath:getClass(className)
    local tIsContainer, tInstance = isKindOf(tClass, "BaseContainer")

    if tIsContainer then
        local tLength = #self._openedContainerStack
        if tLength < 1 then
            if IsLogDebug then
                error("exitPanel :"..className.."; openedContainerStack is empty!")
            end
            return
        end

        local tIndex, tConPre = self:_IndexOfContainer(tClass, self._openedContainerStack)

        if tIndex == tLength then -- 当前界面
            -- 退出当前
            tInstance = self._openedContainerStack[tLength]
            
            if #self._openedContainerStack == tLength then -- 解决在一个container界面onExit中打开另一个container界面异常
                self._openedContainerStack[tLength] = nil
            else
                self:_RemoveContainer(tInstance, self._openedContainerStack)
            end
            tInstance:doExit(false)
            -- 显示上一个
            if not isShowPre then 
                return 
            end
             tConPre = self._openedContainerStack[tLength - 1]
             if tConPre then
                 if tConPre.type == "MainUIContainer" then -- 主界面特殊处理
                    if not tConPre:isClosed() then 
                        tConPre:setVisible(true)
                    else 
                        self:showPanel(GameUIPath.MainUIContainer) 
                    end
                 else
                     if not tConPre:setVisible(true) then 
                        tConPre:reopen() 
                    end -- 直接显示或重新打开
                 end
                 self:cachePanel(tConPre) -- 更新权重
             end
        elseif tIndex ~= -1 then
            tInstance = self._openedContainerStack[tIndex]
            if tInstance then
                tInstance:doExit(false)
                table.remove(self._openedContainerStack, tIndex)
            else
                if IsLogDebug then
                    error("tInstance is nil")
                end
                return
            end
        end

        self:printContainersLog()
    else
        local tContainer = self:getRunningContainer()
        if not tContainer then
            if IsLogDebug then
                error("current running container is nil")
            end
            return
        end
        tInstance = tContainer:getViewItem(tClass)
        if tInstance then
            tInstance:doExit(false, true)
            -- 互斥处理
            local tPanelInner, tData = self:_CheckAlternative(tInstance, false)
            if tPanelInner then
                if self._curAlternative then 
                    self._curAlternative:setVisible(false) 
                end
                tPanelInner:doShow(unpack(tData))
                self._curAlternative = tPanelInner
            end
        end
    end

    if tInstance and tInstance:getFlagDispose() then 
        self:_DestroyPanel(tInstance, tIsContainer) 
    end
end

--getPreUI
function ViewManager:getPreUI()
    local uis = nil
    local tLength = #self._openedContainerStack
    if tLength > 0 then -- 重新打开最上层容器界面
        for i = tLength, 1, -1 do
            local tInstance = self._openedContainerStack[i]
            -- 已打关闭标识或忽略不用重新打开的container。延时关闭
            local tFlagIgnoreReopen = tInstance:checkIsIgnoreReopen()
            if not tFlagIgnoreReopen and not tInstance:getExitFlag() then
                if tInstance and tInstance.type ~= "LoginContainer" then
                    uis = {}
                    local list = tInstance:_ReopenUI(uis)
                    if #list == 0 then
                        uis[#uis+1] = tInstance.type
                    end
                end
                break
            end
        end
    else
        return nil
    end
    return uis
end

-- 关闭所有界面
function ViewManager:closeAllPanel(clear)
    self:printContainersLog()
    local tArr = self._cachedContainers        -- 清理所有打开过的界面
    local tLength, tIndex = #tArr, 1

    local needToRemove = {}
    while tIndex <= tLength do
        local tItem = tArr[tIndex]
        if clear then
            tItem:closeAllPopView(true, true)
        end
        if tItem:getFlagDispose() then
            self:_DestroyPanel(tItem, true)
            tIndex = tIndex - 1
            tLength = tLength - 1
        else
            tItem:doExit(true)
        end
        if clear == true then
            if "MainUIContainer" ~= tItem.type then
                table.insert(needToRemove, tItem.type)
            end
            table.removeValue(self._openedContainerStack, tItem)
        end
        tIndex = tIndex + 1
    end

    for i = 1,#needToRemove do
        for j,v in pairs(self._cachedContainers) do
            if v.type == needToRemove[i] then
                table.remove(self._cachedContainers, j)
            end
        end
    end

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
    local tLength = #self._openedContainerStack
    return self._openedContainerStack[tLength]
end

--- 获取指定容器的上一个容器
function ViewManager:getPreviousContainer(container)
    local tIndex = self:_IndexOfContainer(container, self._openedContainerStack)
    if tIndex > 1 then 
        return self._openedContainerStack[tIndex - 1] 
    end

    return nil
end

-------------------------------------------------------------------------------------------------------------
--
-- @private
--
-------------------------------------------------------------------------------------------------------------

--- 销毁界面
function ViewManager:_DestroyPanel(viewInstance, isContainer)
    local tIsContainer = isContainer == nil and isKindOf(viewInstance, "BaseContainer") or isContainer
    if tIsContainer then -- 待销毁界面为容器
        self:_RemoveContainer(viewInstance, self._cachedContainers)-- 清理容器缓存
        viewInstance:doExit(true)                       -- 销毁容器
        -- 清理打开容器列表
        self:_RemoveContainer(viewInstance, self._openedContainerStack)
    elseif viewInstance.mContainer then
        local tPanel = viewInstance.mContainer:cacheViewItem(viewInstance, false)
        if tPanel then 
            tPanel:doExit(true) 
        end
    end

    -- 有界面销毁，执行GC, 看看会不会引起卡顿
    CResourcesManager.Instance:DoGC()

    self:printContainersLog()
end

-------------------------------------------------------------------------------------------------------------
-- 容器相关
-------------------------------------------------------------------------------------------------------------
--- 显示一个新模块，隐藏上一个
function ViewManager:_PushContainer(container)
    local tList = self._cachedContainers
    local tIndex = self:_IndexOfContainer(container, tList)
    if tIndex == #tList and tList[tIndex]:isVisible() then 
        return 
    end  -- 当前正被显示

    -- -- 隐藏上一个
    -- local tContainer = self:getRunningContainer()
    -- if tContainer then tContainer:setVisible(false) end

    -- 处理缓存的container
    local tInstance
    if tIndex ~= -1 then -- 已存在且不是当前正被显示
        tInstance = tList[tIndex]
    else
        tInstance = container.New()
        table.insert(tList, tInstance)
    end

    -- 处理当前打开的container
    self:_RemoveContainer(tInstance, self._openedContainerStack)
    table.insert(self._openedContainerStack, tInstance)

    return tInstance
end

-- 删除缓存的容器实例
function ViewManager:_RemoveContainer(container, srcArr)
    local tIndex = self:_IndexOfContainer(container, srcArr)
    if tIndex ~= -1 then
        table.remove(srcArr, tIndex)
    end
end

-- 查找制定名模块容器
function ViewManager:_IndexOfContainer(container, srcArr)
    for i, item in ipairs(srcArr) do
        if item.type == container.type then
            return i
        end
    end
    return -1
end

-- 输出容器类log
-- @params isOpened 是否当前正打开的界面。默认true
function ViewManager:printContainersLog(isOpened)
    if not (self.VERBOSE_LEVEL >= 1) or (not IsLogDebug) then 
        return 
    end
    if isOpened == nil or isOpened == false then
        local tmp = {}
        table.insert(tmp, "Cache Containers: ")
        for i, val in ipairs(self._cachedContainers) do
            table.insert(tmp, tostring(val))
        end
        self:print(table.concat(tmp, "\n"))
    end
    if isOpened == nil or isOpened == false then
        local tmp = {}
        table.insert(tmp, "Opening Containers: ")
        for i, val in ipairs(self._openedContainerStack) do
            table.insert(tmp, tostring(val))
        end
        self:print(table.concat(tmp, "\n"))
    end
end
-------------------------------------------------------------------------------------------------------------
-- 界面互斥相关
-------------------------------------------------------------------------------------------------------------
function ViewManager:_CheckAlternative(viewInstance, isAdd, ...)
    if not viewInstance.mIsPop then 
        return 
    end

    local tFlag = self.ALTERNATIVE_POPS[viewInstance.type]
    if tFlag then
        -- 记录数据
        -- self._cacheAlternative[viewInstance] = (isAdd and {view = viewInstance, data = {...}} or nil)
        self._cacheAlternative[viewInstance] = (isAdd and {...} or nil)

        -- 获取优先级值小的界面
        local tMin, tIndex, tPanel = 0, 0
        for key, _ in pairs(self._cacheAlternative) do
            local tValue = self.ALTERNATIVE_POPS[key.type]

            if tValue < tMin or tIndex == 0 then
                tMin = tValue
                tPanel = key
            end
            tIndex = tIndex + 1
        end

        return tPanel, self._cacheAlternative[tPanel]
    end

    return nil
end

-------------------------------------------------------------------------------------------------------------
-- 界面销毁相关
-------------------------------------------------------------------------------------------------------------

--- 缓存新界面。若发生界面替换，返回被替换界面
-- @params isAdd 是否添加缓存数据。默认true
function ViewManager:cachePanel(viewInstance, isAdd)
    isAdd = isAdd ~= false and true or false

    --- 清理缓存数据
    if not isAdd then
        self._dsCached[viewInstance] = nil
        return
    end

    --- 添加缓存数据
    local result

    -- 验证被标记为不参与缓存销毁的界面
    -- local tFlag = self:getRunningContainer():checkFlagCache() or viewInstance:checkFlagCache()
    local tFlag = viewInstance:checkFlagCache()
    if tFlag then 
        return 
    end

    -- 验证是否打开过
    self._dsFlagStart = self._dsFlagStart + 1
    tFlag = self._dsCached[viewInstance]
    if tFlag then                                                    -- 已打开过
        self._dsCached[viewInstance] = self._dsFlagStart
        self:_CacheShowVerbose()
        return
    end

    -- 未打开过
    local tLength = table.nums(self._dsCached)
    if tLength >= self.PANEL_CACHE_LENGTH then                      -- 发生替换
        result = self:_CacheFilter()
        if not result then
            if IsLogDebug then
                error("cachePanel error!")
            end
        end
    end
    self._dsCached[viewInstance] = self._dsFlagStart

    self:_CacheShowVerbose()
    return result
end

--- 查找可被替换项
function ViewManager:_CacheFilter()
    local result

    local tIndex, tMin, tView = 0, 0
    for key, value in pairs(self._dsCached) do
        if value < tMin or tIndex == 0 then
            tMin = value
            tView = key
        end
        tIndex = tIndex + 1
    end

    local tIsContainer, tPanel = isKindOf(tView, "BaseContainer")
    if tIsContainer then                                                                -- 待销毁界面为容器
        if self:getRunningContainer() == tView then           -- 命中的为当前打开的容器
            self._dsFlagStart = self._dsFlagStart + 1
            self._dsCached[tView] = self._dsFlagStart
            result = self:_CacheFilter()
        else
            local tArr = tView:getCachedViews()
            local tLength = #tArr
            if tLength < 1 then                       -- 销毁容器本身
                result = tView
            else                                      -- 销毁容器内一个界面
                tIndex = 1
                while tIndex <= tLength do
                    tPanel = tArr[tIndex]
                    if self._dsCached[tPanel] then
                        result = tPanel
                        break
                    end
                    tIndex = tIndex + 1
                end
                if not result then
                    self._dsFlagStart = self._dsFlagStart + 1
                    self._dsCached[tView] = self._dsFlagStart
                    result = self:_CacheFilter()
                end
            end
        end
    else
        result = tView
    end

    return result
end

--- 打印销毁相关数据
function ViewManager:_CacheShowVerbose()
    if IsLogDebug then
        return
    end
    if not (self.VERBOSE_LEVEL >= 2) then 
        return 
    end

    -- 打印缓存界面信息
    local tTable = {}
    for key, value in pairs(self._dsCached) do
        tTable[key.type] = value
    end
    dump(tTable)
end

--log输出
function ViewManager:print(str)
    if IsLogDebug then
        log(str, "[ViewManager]>> ")
    end
end

return ViewManager
