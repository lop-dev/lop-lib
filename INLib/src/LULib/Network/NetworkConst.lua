---------------------------------------------------------------------------------
--      This code was created by League of Perfect (lop_dev@hotmail.com)
--      You can get the latest version from : github.com/lop-dev/lop-lib
---------------------------------------------------------------------------------

NetworkConst = {}

NetworkConst.EServerType =
{
    ELGCSERVER_UNKNOW = 0,
    -- 组内服务器类型
    ELGCSERVER_GAMECLIENT = 8,      -- "GC" -- fix
    ESERVER_MASTER = 9,             -- "MS"
    ESERVER_LOGIC_START = 10,
    ELGCSERVER_GATEWAY = 10,        -- "GW" -- fix
    ESERVER_DATABASE = 11,          -- "DB"
    ELGCSERVER_LOGIN = 12,          -- "LG" -- fix
    ELGCSERVER_CENTER = 13,         -- "CT" -- fix
    ESERVER_LOG = 14,               -- "LO"
    ELGCSERVER_GUILD = 17,          -- GU
}

NetworkConst.EMsgType =
{
    -- CSLib.Message.EMT_EXTEND
    EMT_ACCOUNTSERVER = 256,
    EMT_ACCOUNTCLIENT = 257,
    EMT_EXTEND = 258,
}

NetworkConst.EMsgIDAccountServer =
{
    EMID_C2S_REQ_VERIFY_NAME_PWD = 200,
    EMID_S2C_RES_VERIFY_NAME_PWD = 201,

    EMID_C2S_REQ_INSERT_NAME_PWD = 202,
    EMID_S2C_RES_INSERT_NAME_PWD = 203,
}

return NetworkConst
