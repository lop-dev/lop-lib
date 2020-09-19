---------------------------------------------------------------------------------
--      This code was created by League of Perfect (lop_dev@hotmail.com)
--      You can get the latest version from : github.com/lop-dev/lop-lib
---------------------------------------------------------------------------------

--- 浮点数进度处理
--@params precision 保留的浮点数小数部分位数
function math.fixUp(value, precision)
    return math.round(value * math.pow(10, precision)) / math.pow(10, precision)
end

function math.round(value)
    value = checkNumber(value)
    return math.floor(value + 0.5)
end