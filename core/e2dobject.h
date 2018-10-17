// Copyright (c) 2016-2018 Easy2D - Nomango
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#ifndef __E2D_OBJECT_H__
#define __E2D_OBJECT_H__


#include "e2dutil.h"
#include "e2devent.h"


namespace easy2d
{

	// 图片
	class Image
		: public Ref
	{
	public:
		Image();

		explicit Image(
			const Resource& res
		);

		explicit Image(
			const Resource& res,
			const Rect& crop_rect	/* 裁剪矩形 */
		);

		explicit Image(
			const String& file_name
		);

		explicit Image(
			const String& file_name,
			const Rect& crop_rect	/* 裁剪矩形 */
		);

		virtual ~Image();

		// 加载图片资源
		bool Load(
			const Resource& res
		);

		// 加载图片资源
		bool Load(
			const String& file_name
		);

		// 将图片裁剪为矩形
		void Crop(
			const Rect& crop_rect	/* 裁剪矩形 */
		);

		// 获取宽度
		float GetWidth() const;

		// 获取高度
		float GetHeight() const;

		// 获取大小
		Size GetSize() const;

		// 获取源图片宽度
		float GetSourceWidth() const;

		// 获取源图片高度
		float GetSourceHeight() const;

		// 获取源图片大小
		Size GetSourceSize() const;

		// 获取裁剪位置 X 坐标
		float GetCropX() const;

		// 获取裁剪位置 Y 坐标
		float GetCropY() const;

		// 获取裁剪位置
		Point GetCropPos() const;

		// 获取裁剪矩形
		const Rect& GetCropRect() const;

		// 获取 ID2D1Bitmap 对象
		ID2D1Bitmap * GetBitmap() const;

		// 清空缓存
		static void ClearCache();

	private:
		E2D_DISABLE_COPY(Image);

		// 缓存 Bitmap 资源
		static bool CacheBitmap(
			const String& file_name
		);

		// 缓存 Bitmap 资源
		static bool CacheBitmap(
			const Resource& res
		);

		// 设置 Bitmap
		void SetBitmap(
			ID2D1Bitmap * bitmap
		);

	private:
		Rect crop_rect_;
		ID2D1Bitmap * bitmap_;

		static std::map<size_t, ID2D1Bitmap*> bitmap_cache_;
	};


	class Node;

	// 场景
	class Scene
		: public Ref
	{
	public:
		Scene();

		explicit Scene(
			Node * root
		);

		virtual ~Scene();

		// 进入场景
		virtual void OnEnter() {}

		// 退出场景
		virtual void OnExit() {}

		// 更新场景
		virtual void OnUpdate(float dt) {}

		// 设置根节点
		void SetRoot(
			Node * root
		);

		// 获取根节点
		Node* GetRoot() const;

		// 渲染场景
		void Draw();

		// 分发鼠标消息
		virtual void Dispatch(
			const MouseEvent& e
		);

		// 分发按键消息
		virtual void Dispatch(
			const KeyEvent& e
		);

		// 设置转换矩阵
		void SetTransform(
			const D2D1::Matrix3x2F& matrix
		);

		// 获取转换矩阵
		const D2D1::Matrix3x2F& GetTransform() const;

	private:
		E2D_DISABLE_COPY(Scene);

	private:
		Node*				root_;
		D2D1::Matrix3x2F	transform_;
	};


	// 定时任务
	class Task
		: public Ref
	{
		friend class Node;

	public:
		explicit Task(
			const Function& func,		/* 执行函数 */
			const String& name = L""	/* 任务名称 */
		);

		explicit Task(
			const Function& func,		/* 执行函数 */
			float delay,				/* 时间间隔（秒） */
			int times = -1,				/* 执行次数（设 -1 为永久执行） */
			const String& name = L""	/* 任务名称 */
		);

		// 启动任务
		void Start();

		// 停止任务
		void Stop();

		// 任务是否正在执行
		bool IsRunning() const;

		// 获取任务名称
		const String& GetName() const;

		// 任务是否就绪
		bool IsReady() const;

		// 执行任务
		void Update();

		// 重置计时
		void ResetTime();

	private:
		bool		running_;
		bool		stopped_;
		int			run_times_;
		int			total_times_;
		String		name_;
		Duration	delay_;
		Time		last_time_;
		Function	callback_;
		Node *		target_;
	};


	// 按键消息处理接口
	class KeyEventHandler
	{
	public:
		// 处理按键消息
		virtual void Handle(KeyEvent e) = 0;
	};


	// 鼠标消息处理接口
	class MouseEventHandler
	{
	public:
		// 处理鼠标消息
		virtual void Handle(MouseEvent e) = 0;
	};


	class Action;

	// 节点
	class Node
		: public Ref
	{
		friend class Game;
		friend class Scene;

	public:
		typedef std::vector<Node*> Nodes;
		typedef std::vector<Action*> Actions;
		typedef std::vector<Task*> Tasks;

		Node();

		virtual ~Node();

		// 渲染节点
		virtual void OnDraw() const {}

		// 更新节点
		virtual void OnUpdate(float dt) {}

		// 获取节点显示状态
		bool IsVisible() const;

		// 获取节点名称
		const String& GetName() const;

		// 获取节点名称的 Hash 值
		size_t GetHashName() const;

		// 获取节点绘图顺序
		int GetOrder() const;

		// 获取节点坐标
		const Point& GetPosition() const;

		// 获取节点宽度
		float GetWidth() const;

		// 获取节点高度
		float GetHeight() const;

		// 获取节点宽度（不考虑缩放）
		float GetRealWidth() const;

		// 获取节点高度（不考虑缩放）
		float GetRealHeight() const;

		// 获取节点大小（不考虑缩放）
		const Size& GetRealSize() const;

		// 获取节点的支点
		float GetPivotX() const;

		// 获取节点的支点
		float GetPivotY() const;

		// 获取节点大小
		Size GetSize() const;

		// 获取节点横向缩放比例
		float GetScaleX() const;

		// 获取节点纵向缩放比例
		float GetScaleY() const;

		// 获取节点横向倾斜角度
		float GetSkewX() const;

		// 获取节点纵向倾斜角度
		float GetSkewY() const;

		// 获取节点旋转角度
		float GetRotation() const;

		// 获取二维转换矩阵
		const Transform& GetTransform() const;

		// 获取节点透明度
		float GetOpacity() const;

		// 获取显示透明度
		float GetDisplayOpacity() const;

		// 获取父节点
		Node * GetParent() const;

		// 获取节点所在场景
		Scene * GetParentScene() const;

		// 设置节点是否显示
		void SetVisible(
			bool value
		);

		// 设置节点名称
		void SetName(
			const String& name
		);

		// 设置节点横坐标
		virtual void SetPositionX(
			float x
		);

		// 设置节点纵坐标
		virtual void SetPositionY(
			float y
		);

		// 设置节点坐标
		virtual void SetPosition(
			const Point & point
		);

		// 设置节点坐标
		virtual void SetPosition(
			float x,
			float y
		);

		// 移动节点
		virtual void MoveBy(
			float x,
			float y
		);

		// 移动节点
		virtual void MoveBy(
			const Point & vector
		);

		// 设置节点绘图顺序
		// 默认为 0
		virtual void SetOrder(
			int order
		);

		// 设置横向缩放比例
		// 默认为 1.0
		virtual void SetScaleX(
			float scale_x
		);

		// 设置纵向缩放比例
		// 默认为 1.0
		virtual void SetScaleY(
			float scale_y
		);

		// 设置缩放比例
		// 默认为 (1.0, 1.0)
		virtual void SetScale(
			float scale_x,
			float scale_y
		);

		// 设置缩放比例
		// 默认为 1.0
		virtual void SetScale(
			float scale
		);

		// 设置横向倾斜角度
		// 默认为 0
		virtual void SetSkewX(
			float skew_x
		);

		// 设置纵向倾斜角度
		// 默认为 0
		virtual void SetSkewY(
			float skew_y
		);

		// 设置倾斜角度
		// 默认为 (0, 0)
		virtual void SetSkew(
			float skew_x,
			float skew_y
		);

		// 设置旋转角度
		// 默认为 0
		virtual void SetRotation(
			float rotation
		);

		// 设置透明度
		// 默认为 1.0, 范围 [0, 1]
		virtual void SetOpacity(
			float opacity
		);

		// 设置支点的横向位置
		// 默认为 0, 范围 [0, 1]
		virtual void SetPivotX(
			float pivot_x
		);

		// 设置支点的纵向位置
		// 默认为 0, 范围 [0, 1]
		virtual void SetPivotY(
			float pivot_y
		);

		// 设置支点位置
		// 默认为 (0, 0), 范围 [0, 1]
		virtual void SetPivot(
			float pivot_x,
			float pivot_y
		);

		// 修改节点宽度
		virtual void SetWidth(
			float width
		);

		// 修改节点高度
		virtual void SetHeight(
			float height
		);

		// 修改节点大小
		virtual void SetSize(
			float width,
			float height
		);

		// 修改节点大小
		virtual void SetSize(
			const Size & size
		);

		// 设置二维转换
		virtual void SetTransform(
			const Transform& transform
		);

		// 启用或关闭渲染区域裁剪
		virtual void SetClipEnabled(
			bool enabled
		);

		// 设置节点边缘颜色
		virtual void SetBorderColor(
			const Color& color
		);

		// 判断点是否在节点内
		bool ContainsPoint(
			const Point& point
		);

		// 判断两物体是否相交
		bool Intersects(
			Node * node
		);

		// 添加子节点
		void AddChild(
			Node * child,
			int order = 0		/* 渲染顺序 */
		);

		// 添加多个子节点
		void AddChild(
			const Nodes& nodes,	/* 节点数组 */
			int order = 0		/* 渲染顺序 */
		);

		// 获取所有名称相同的子节点
		Nodes GetChildren(
			const String& name
		) const;

		// 获取名称相同的子节点
		Node* GetChild(
			const String& name
		) const;

		// 获取所有子节点
		const Nodes& GetAllChildren() const;

		// 获取子节点数量
		int GetChildrenCount() const;

		// 移除子节点
		bool RemoveChild(
			Node * child
		);

		// 移除所有名称相同的子节点
		void RemoveChildren(
			const String& child_name
		);

		// 移除所有节点
		void RemoveAllChildren();

		// 从父节点移除
		void RemoveFromParent();

		// 执行动作
		void RunAction(
			Action * action
		);

		// 继续动作
		void ResumeAction(
			const String& name
		);

		// 暂停动作
		void PauseAction(
			const String& name
		);

		// 停止动作
		void StopAction(
			const String& name
		);

		// 继续所有暂停动作
		void ResumeAllActions();

		// 暂停所有动作
		void PauseAllActions();

		// 停止所有动作
		void StopAllActions();

		// 获取所有动作
		const Actions& GetAllActions() const;

		// 添加任务
		void AddTask(
			Task * task
		);

		// 启动任务
		void StartTasks(
			const String& task_name
		);

		// 停止任务
		void StopTasks(
			const String& task_name
		);

		// 移除任务
		void RemoveTasks(
			const String& task_name
		);

		// 启动所有任务
		void StartAllTasks();

		// 停止所有任务
		void StopAllTasks();

		// 移除所有任务
		void RemoveAllTasks();

		// 获取所有任务
		const Tasks& GetAllTasks() const;

	protected:
		// 遍历节点
		virtual void Visit();

		// 分发鼠标消息
		virtual bool Dispatch(
			const MouseEvent& e,
			bool handled
		);

		// 分发按键消息
		virtual bool Dispatch(
			const KeyEvent& e,
			bool handled
		);

	private:
		E2D_DISABLE_COPY(Node);

		// 渲染节点边缘
		void DrawBorder();

		// 设置节点所在场景
		void SetParentScene(
			Scene * scene
		);

		// 更新子节点
		void UpdateChildren(float dt);

		// 更新转换矩阵
		void UpdateTransform();

		// 更新节点透明度
		void UpdateOpacity();

		// 更新动作
		void UpdateActions();

		// 更新任务
		void UpdateTasks();

		// 更新节点时间
		void UpdateTime();

	private:
		String				name_;
		size_t				hash_name_;
		Transform			transform_;
		float				display_opacity_;
		float				real_opacity_;
		int					order_;
		bool				visible_;
		bool				clip_enabled_;
		bool				dirty_sort_;
		bool				dirty_transform_;
		Scene*				parent_scene_;
		Node*				parent_;
		Color				border_color_;
		Actions				actions_;
		Tasks				tasks_;
		Nodes				children_;
		ID2D1Geometry*		border_;
		D2D1::Matrix3x2F	initial_matrix_;
		D2D1::Matrix3x2F	final_matrix_;
	};


	// 精灵
	class Sprite
		: public Node
	{
	public:
		Sprite();

		explicit Sprite(
			Image * image
		);

		explicit Sprite(
			const Resource& res
		);

		explicit Sprite(
			const Resource& res,
			const Rect& crop_rect	/* 裁剪矩形 */
		);

		explicit Sprite(
			const String& file_name
		);

		explicit Sprite(
			const String& file_name,
			const Rect& crop_rect	/* 裁剪矩形 */
		);

		virtual ~Sprite();

		// 加载图片文件
		bool Load(
			const Resource& res
		);

		// 加载图片文件
		bool Load(
			const String& file_name
		);

		// 加载图片
		bool Load(
			Image * image
		);

		// 将图片裁剪为矩形
		void Crop(
			const Rect& crop_rect	/* 裁剪矩形 */
		);

		// 获取 Image 对象
		Image * GetImage() const;

		// 渲染精灵
		virtual void OnDraw() const override;

	private:
		E2D_DISABLE_COPY(Sprite);

	private:
		Image* image_;
	};


	// 文本
	class Text
		: public Node
	{
	public:
		// 文本对齐方式
		enum class Align
		{
			Left,		/* 左对齐 */
			Right,		/* 右对齐 */
			Center		/* 居中对齐 */
		};

		// 文本样式
		class Style
		{
		public:
			Color		color;				// 颜色
			Align		alignment;			// 对齐方式
			bool		wrap;				// 打开自动换行
			float		wrap_width;			// 自动换行宽度
			float		line_spacing;		// 行间距
			bool		underline;			// 下划线
			bool		strikethrough;		// 删除线
			bool		outline;			// 显示描边
			Color		outline_color;		// 描边颜色
			float		outline_width;		// 描边线宽
			Stroke		outline_stroke;		// 描边线相交样式

		public:
			Style();

			Style(
				Color color,
				Align alignment = Align::Left,
				bool wrap = false,
				float wrap_width = 0.f,
				float line_spacing = 0.f,
				bool underline = false,
				bool strikethrough = false,
				bool outline = true,
				Color outline_color = Color(Color::Black, 0.5),
				float outline_width = 1.f,
				Stroke outline_stroke = Stroke::Round
			);
		};

	public:
		Text();

		explicit Text(
			const String& text,						/* 文字内容 */
			const Font& font = Font(),				/* 字体 */
			const Style& style = Style()			/* 文本样式 */
		);

		virtual ~Text();

		// 获取文本
		const String& GetText() const;

		// 获取字体
		const Font& GetFont() const;

		// 获取文本样式
		const Style& GetStyle() const;

		// 获取字体族
		const String& GetFontFamily() const;

		// 获取当前字号
		float GetFontSize() const;

		// 获取当前字体粗细值
		UINT GetFontWeight() const;

		// 获取文字颜色
		const Color& GetColor() const;

		// 获取描边颜色
		const Color& GetOutlineColor() const;

		// 获取描边线宽
		float GetOutlineWidth() const;

		// 获取描边线相交样式
		Stroke GetOutlineStroke() const;

		// 获取文本显示行数
		int GetLineCount() const;

		// 是否是斜体
		bool IsItalic() const;

		// 是否显示删除线
		bool strikethrough() const;

		// 是否显示下划线
		bool underline() const;

		// 是否显示描边
		bool outline() const;

		// 设置文本
		void SetText(
			const String& text
		);

		// 设置文本样式
		void SetStyle(
			const Style& style
		);

		// 设置字体
		void SetFont(
			const Font& font
		);

		// 设置字体族
		void SetFontFamily(
			const String& family
		);

		// 设置字号（默认值为 22）
		void SetFontSize(
			float size
		);

		// 设置字体粗细值（默认值为 Text::Font::Weight::Normal）
		void SetFontWeight(
			UINT weight
		);

		// 设置文字颜色（默认值为 Color::WHITE）
		void SetColor(
			Color color
		);

		// 设置文字斜体（默认值为 false）
		void SetItalic(
			bool value
		);

		// 打开或关闭文本自动换行（默认为关闭）
		void SetWrapEnabled(
			bool wrap
		);

		// 设置文本自动换行的宽度（默认为 0）
		void SetWrapWidth(
			float wrap_width
		);

		// 设置行间距（默认为 0）
		void SetLineSpacing(
			float line_spacing
		);

		// 设置对齐方式（默认为 Align::Left）
		void SetAlignment(
			Align align
		);

		// 设置下划线（默认值为 false）
		void SetUnderline(
			bool underline
		);

		// 设置删除线（默认值为 false）
		void SetStrikethrough(
			bool strikethrough
		);

		// 设置是否显示描边
		void SetOutline(
			bool outline
		);

		// 设置描边颜色
		void SetOutlineColor(
			Color outline_color
		);

		// 设置描边线宽
		void SetOutlineWidth(
			float outline_width
		);

		// 设置描边线相交样式
		void SetOutlineStroke(
			Stroke outline_stroke
		);

		// 渲染文字
		virtual void OnDraw() const override;

	private:
		E2D_DISABLE_COPY(Text);

		// 重新排版文字
		void Reset();

		// 创建文字格式化
		void CreateFormat();

		// 创建文字布局
		void CreateLayout();

	private:
		String				text_;
		Font				font_;
		Style				style_;
		IDWriteTextFormat*	text_format_;
		IDWriteTextLayout*	text_layout_;
	};


	// 画布
	class Canvas
		: public Node
	{
	public:
		Canvas(
			float width,
			float height
		);

		virtual ~Canvas();

		// 设置线条颜色
		void SetLineColor(
			const Color& color
		);

		// 设置填充颜色
		void SetFillColor(
			const Color& color
		);

		// 设置线条宽度
		void SetStrokeWidth(
			float width
		);

		// 设置线条相交样式
		void SetStrokeStyle(
			Stroke strokeStyle
		);

		// 获取线条颜色
		Color GetLineColor() const;

		// 获取填充颜色
		Color GetFillColor() const;

		// 获取线条宽度
		float GetStrokeWidth() const;

		// 获取线条相交样式
		Stroke GetStrokeStyle() const;

		// 画直线
		void DrawLine(
			const Point& begin,
			const Point& end
		);

		// 画圆形边框
		void DrawCircle(
			const Point& center,
			float radius
		);

		// 画椭圆形边框
		void DrawEllipse(
			const Point& center,
			float radius_x,
			float radius_y
		);

		// 画矩形边框
		void DrawRect(
			const Rect& rect
		);

		// 画圆角矩形边框
		void DrawRoundedRect(
			const Rect& rect,
			float radius_x,
			float radius_y
		);

		// 填充圆形
		void FillCircle(
			const Point& center,
			float radius
		);

		// 填充椭圆形
		void FillEllipse(
			const Point& center,
			float radius_x,
			float radius_y
		);

		// 填充矩形
		void FillRect(
			const Rect& rect
		);

		// 填充圆角矩形
		void FillRoundedRect(
			const Rect& rect,
			float radius_x,
			float radius_y
		);

	private:
		E2D_DISABLE_COPY(Canvas);

	private:
		float					stroke_width_;
		Stroke					stroke_;
		ID2D1RenderTarget*		render_target_;
		ID2D1SolidColorBrush*	fill_brush_;
		ID2D1SolidColorBrush*	line_brush_;
		ID2D1StrokeStyle*		stroke_style_;
	};

} // end of easy2d namespace


#endif // __E2D_OBJECT_H__
