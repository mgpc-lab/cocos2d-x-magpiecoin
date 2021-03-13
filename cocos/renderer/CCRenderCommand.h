/****************************************************************************
 Copyright (c) 2013-2016 Chukong Technologies Inc.
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.

 http://www.cocos2d-x.org

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/
#pragma once

#include "platform/CCPlatformMacros.h"
#include "base/ccTypes.h"
#include "renderer/CCPipelineDescriptor.h"

/**
 * @addtogroup renderer
 * @{
 */

NS_CC_BEGIN

/** Base class of the `RenderCommand` hierarchy.
*
 The `Renderer` knows how to render `RenderCommands` objects.
 */

    //第二、RenderCommand的类型

    //这里的类型讲解主要参考这篇文章中关于RenderComman的类型讲解。

    //QUAD_COMMAND：QuadCommand类绘制精灵等。所有绘制图片的命令都会调用到这里，处理这个类型命令的代码就是绘制贴图的openGL代码，下一篇文章会详细介绍这部分代码。

    //CUSTOM_COMMAND：CustomCommand类自定义绘制，自己定义绘制函数，在调用绘制时只需调用已经传进来的回调函数就可以，裁剪节点，绘制图形节点都采用这个绘制，把绘制函数定义在自己的类里。这种类型的绘制命令不会在处理命令的时候调用任何一句openGL代码，而是调用你写好并设置给func的绘制函数，后续文章会介绍引擎中的所有自定义绘制，并自己实现一个自定义的绘制。

    //BATCH_COMMAND：BatchCommand类批处理绘制，批处理精灵和粒子。其实它类似于自定义绘制，也不会再render函数中出现任何一句openGL函数，它调用一个固定的函数，这个函数会在下一篇文章中介绍。

    //GROUP_COMMAND：GroupCommand类绘制组，一个节点包括两个以上绘制命令的时候，把这个绘制命令存储到另外一个_renderGroups中的元素中，并把这个元素的指针作为一个节点存储到_renderGroups[0]中。




class CC_DLL RenderCommand
{
public:
    /**Enum the type of render command. */
    enum class Type
    {
        /** Reserved type.*/
        UNKNOWN_COMMAND,
        /** Quad command, used for draw quad.*/
        QUAD_COMMAND,
        /**Custom command, used to draw things other then TRIANGLES_COMMAND.*/
        CUSTOM_COMMAND,
        /**Group command, which can group command in a tree hierarchy.*/
        GROUP_COMMAND,
        /**Mesh command, used to draw 3D meshes.*/
        MESH_COMMAND,
        /**Triangles command, used to draw triangles.*/
        TRIANGLES_COMMAND,
        /**Callback command, used for calling callback for rendering.*/
        CALLBACK_COMMAND,
        CAPTURE_SCREEN_COMMAND
    };

    /**
     Init function, will be called by all the render commands.
     @param globalZOrder The global order of command, used for rendercommand sorting.
     @param modelViewTransform Modelview matrix when submitting the render command.
     @param flags Flag used to indicate whether the command should be draw at 3D mode or not.
     */
    void init(float globalZOrder, const Mat4& modelViewTransform, unsigned int flags);
    
    /** Get global Z order. */
    float getGlobalOrder() const { return _globalOrder; }

    /** Returns the Command type. */
    Type getType() const { return _type; }
    
    /** Returns whether is transparent. */
    bool isTransparent() const { return _isTransparent; }
    
    /** Set transparent flag. */
    void setTransparent(bool isTransparent) { _isTransparent = isTransparent; }
    /**
     Get skip batching status, if a rendering is skip batching, it will be forced to be rendering separately.
     */
    bool isSkipBatching() const { return _skipBatching; }
    /**Set skip batching.*/
    void setSkipBatching(bool value) { _skipBatching = value; }
    /**Whether the command should be rendered at 3D mode.*/
    bool is3D() const { return _is3D; }
    /**Set the command rendered in 3D mode or not.*/
    void set3D(bool value) { _is3D = value; }
    /**Get the depth by current model view matrix.*/
    float getDepth() const { return _depth; }
    /// Can use the result to change the descriptor content.
    inline PipelineDescriptor& getPipelineDescriptor() { return _pipelineDescriptor; }

    const Mat4 & getMV() const { return _mv; }

protected:
    /**Constructor.*/
    RenderCommand();
    /**Destructor.*/
    virtual ~RenderCommand();
    //used for debug but it is not implemented.
    void printID();

    /**Type used in order to avoid dynamic cast, faster. */
    Type _type = RenderCommand::Type::UNKNOWN_COMMAND;

    /** Commands are sort by global Z order. */
    float _globalOrder = 0.f;
    
    /** Transparent flag. */
    bool  _isTransparent = true;
    
    /**
     QuadCommand and TrianglesCommand could be auto batched if there material ID is the same, however, if
     a command is skip batching, it would be forced to draw in a separate function call, and break the batch.
     */
    bool _skipBatching = false;
    
    /** Is the command been rendered on 3D pass. */
    bool _is3D = false;
    
    /** Depth from the model view matrix.*/
    float _depth = 0.f;

    Mat4 _mv;

    PipelineDescriptor _pipelineDescriptor;
};

NS_CC_END
/**
 end of support group
 @}
 */
