/****************************************************************************
 Copyright (c) 2013 cocos2d-x.org
 
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

#include "UIHelper.h"
#include "json/DictionaryHelper.h"
#include "cocos2d.h"
#include "reader/CCSGUIReader.h"

using namespace cocos2d;
using namespace cocostudio;

namespace gui {
    
static UIHelper* helperInstance = NULL;

UIHelper* UIHelper::instance()
{
    if (!helperInstance)
    {
        helperInstance = new UIHelper();
    }
    return helperInstance;
}

void UIHelper::purgeUIHelper()
{
	CC_SAFE_DELETE(helperInstance);
}

UIHelper::UIHelper():
_textureFiles(NULL),
_fileDesignHeight(0.0f),
_fileDesignWidth(0.0f)
{
    Size winSize = Director::getInstance()->getWinSize();
    _fileDesignWidth = winSize.width;
    _fileDesignHeight = winSize.height;
    init();
}

UIHelper::~UIHelper()
{
    cocostudio::CCSGUIReader::purgeCCSGUIReader();
}

void UIHelper::init()
{
    _textureFiles = CCArray::create();
    _textureFiles->retain();
}

UIWidget* UIHelper::createWidgetFromJsonFile(const char *fileName)
{
    return CCSGUIReader::shareReader()->widgetFromJsonFile(fileName);
}

void UIHelper::addSpriteFrame(const char *fileName)
{
    if (!fileName || strcmp(fileName, "") == 0)
    {
        return;
    }
    ccArray* arrayTextures = _textureFiles->data;
    int length = arrayTextures->num;
    for (int i=0;i<length;i++)
    {
        String* file = (String*)(arrayTextures->arr[i]);
        if (strcmp(file->_string.c_str(), fileName) == 0)
        {
            return;
        }
    }
    _textureFiles->addObject(CCString::create(fileName));
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile(fileName);
}

void UIHelper::removeSpriteFrame(const char *fileName)
{
    if (!fileName || strcmp(fileName, "") == 0)
    {
        return;
    }
    ccArray* arrayTextures = _textureFiles->data;
    int length = arrayTextures->num;
    for (int i=0;i<length;i++)
    {
        String* file = (String*)(arrayTextures->arr[i]);
        if (strcmp(file->_string.c_str(), fileName) == 0)
        {
            SpriteFrameCache::getInstance()->removeSpriteFrameByName(fileName);
            _textureFiles->removeObject(file);
            return;
        }
    }
}

void UIHelper::removeAllSpriteFrame()
{
    ccArray* arrayTextures = _textureFiles->data;
    int length = arrayTextures->num;
    for (int i=0;i<length;i++)
    {
        String* file = (String*)(arrayTextures->arr[i]);
        SpriteFrameCache::getInstance()->removeSpriteFrameByName(file->_string.c_str());
    }
    _textureFiles->removeAllObjects();
}

UIWidget* UIHelper::seekWidgetByTag(UIWidget* root, int tag)
{
    if (!root)
    {
        return NULL;
    }
    if (root->getTag() == tag)
    {
        return root;
    }
    ccArray* arrayRootChildren = root->getChildren()->data;
    int length = arrayRootChildren->num;
    for (int i=0;i<length;i++)
    {
        UIWidget* child = (UIWidget*)(arrayRootChildren->arr[i]);
        UIWidget* res = seekWidgetByTag(child,tag);
        if (res != NULL)
        {
            return res;
        }
    }
    return NULL;
}

UIWidget* UIHelper::seekWidgetByName(UIWidget* root, const char *name)
{
    if (!root)
    {
        return NULL;
    }
    if (strcmp(root->getName(), name) == 0)
    {
        return root;
    }
    ccArray* arrayRootChildren = root->getChildren()->data;
    int length = arrayRootChildren->num;
    for (int i=0;i<length;i++)
    {
        UIWidget* child = (UIWidget*)(arrayRootChildren->arr[i]);
        UIWidget* res = seekWidgetByName(child,name);
        if (res != NULL)
        {
            return res;
        }
    }
    return NULL;
}

UIWidget* UIHelper::seekWidgetByRelativeName(UIWidget *root, const char *name)
{
    if (!root)
    {
        return NULL;
    }
    ccArray* arrayRootChildren = root->getChildren()->data;
    int length = arrayRootChildren->num;
    for (int i=0;i<length;i++)
    {
        UIWidget* child = (UIWidget*)(arrayRootChildren->arr[i]);
        RelativeLayoutParameter* layoutParameter = dynamic_cast<RelativeLayoutParameter*>(child->getLayoutParameter());
        if (layoutParameter && strcmp(layoutParameter->getRelativeName(), name) == 0)
        {
            return child;
        }
    }
    return NULL;
}

void UIHelper::setFileDesignWidth(float width)
{
    _fileDesignWidth = width;
}

float UIHelper::getFileDesignWidth()
{
    return _fileDesignWidth;
}

void UIHelper::setFileDesignHeight(float height)
{
    _fileDesignHeight = height;
}

float UIHelper::getFileDesignHeight()
{
    return _fileDesignHeight;
}

/*temp action*/
UIWidget* UIHelper::seekActionWidgetByActionTag(UIWidget* root, int tag)
{
	if (!root)
	{
		return NULL;
	}
	if (root->getActionTag() == tag)
	{
		return root;
	}
    ccArray* arrayRootChildren = root->getChildren()->data;
    int length = arrayRootChildren->num;
	for (int i=0;i<length;i++)
	{
		UIWidget* child = (UIWidget*)(arrayRootChildren->arr[i]);
		UIWidget* res = seekActionWidgetByActionTag(child,tag);
		if (res != NULL)
		{
			return res;
		}
	}
	return NULL;
}

}