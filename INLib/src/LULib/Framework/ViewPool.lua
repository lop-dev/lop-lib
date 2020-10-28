---------------------------------------------------------------------------------
--      This code was created by League of Perfect (lop_dev@hotmail.com)
--      You can get the latest version from : github.com/lop-dev/lop-lib
--      界面对象prefab缓存池,用于界面预加载
---------------------------------------------------------------------------------

local ViewPool = {}

ViewPool._cacheObjPool = {} -- 界面prefab对象缓存池，用于预加载

-- 预加载界面
function ViewPool:preLoadView(fullClassName)
    if  self._cacheObjPool[fullClassName] ~= nil then
        return
    end
    if IsLogDebug then
        print("[ViewPool]>> preLoadView: "..fullClassName)
    end
    local tClass = GameUIPath:getClass(fullClassName)
    if tClass then
        CommonUtil:getResourcePrefabClone(ERESOURCE_TYPE.UI_PREFABS, tClass.PREFAB_PATH, true,
        function(obj)
            self._cacheObjPool[tClass.type] = obj
            obj:SetActive(false)
            CUtilObj2Lua.SetParent(obj, UGUI.UICache, true)
        end)
    end
end

-- 清除所有缓存
function ViewPool:clearPool()
    if self._cacheObjPool == nil then
        return
    end

    for _, v in pairs(self._cacheObjPool) do
        if v ~= nil then
            GameObject.Destroy(v)
        end
    end

    self._cacheObjPool = {}
end

-- 判断界面是否预加载
function ViewPool:isViewPreload(shortClassName)
    return self._cacheObjPool[shortClassName] ~= nil
end

-- 弹出预加载界面
function ViewPool:popView(shortClassName)
    local view = self._cacheObjPool[shortClassName]
    self._cacheObjPool[shortClassName] = nil
    return view
end

return ViewPool
