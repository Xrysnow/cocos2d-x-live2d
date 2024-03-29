# cocos2d-x-live2d

This project provides Live2D support for [cocos2d-x](https://github.com/cocos2d/cocos2d-x) v4 and [this project](https://github.com/Xrysnow/cocos2d-x).

You need to download Cubism SDK for Native from [here](https://www.live2d.com/en/download/cubism-sdk/download-native) and copy files in `Core` folder in the SDK to `Core` folder in this project. For license of usage, please check [here](https://github.com/Live2D/CubismNativeSamples/blob/develop/LICENSE.md).

This project is base on [Cubism Native Samples](https://github.com/Live2D/CubismNativeSamples) and published under [Live2D Open Software License](https://www.live2d.com/eula/live2d-open-software-license-agreement_en.html).

### Note

Branch v3 can work with cocos2d-x v3, but will not be maintained any more.

## Example in Lua

```lua
local model = l2d.Model:create('Haru', 'Haru.model3.json')
scene:addChild(model)
model:setPosition(cc.p(500, 500))
model:setTouchEnabled(true)
model:setAutoDragging(true)
model:addClickEventListener(function()
    local pos = model:getTouchEndPosition()
    if model:areaHitTest('Head', pos.x, pos.y) then
        print('hit at head')
    end
end)
```

Remember to call `l2d::Framework::end()` in C++ to end the framework.
