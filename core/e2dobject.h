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

	// ͼƬ
	class Image
		: public Ref
	{
	public:
		Image();

		explicit Image(
			Resource& res
		);

		explicit Image(
			Resource& res,
			const Rect& crop_rect	/* �ü����� */
		);

		explicit Image(
			const std::wstring& file_name
		);

		explicit Image(
			const std::wstring& file_name,
			const Rect& crop_rect	/* �ü����� */
		);

		virtual ~Image();

		// ����ͼƬ��Դ
		bool Load(
			Resource& res
		);

		// ����ͼƬ��Դ
		bool Load(
			const std::wstring& file_name
		);

		// ��ͼƬ�ü�Ϊ����
		void Crop(
			const Rect& crop_rect	/* �ü����� */
		);

		// ��ȡ���
		float GetWidth() const;

		// ��ȡ�߶�
		float GetHeight() const;

		// ��ȡ��С
		Size GetSize() const;

		// ��ȡԴͼƬ���
		float GetSourceWidth() const;

		// ��ȡԴͼƬ�߶�
		float GetSourceHeight() const;

		// ��ȡԴͼƬ��С
		Size GetSourceSize() const;

		// ��ȡ�ü�λ�� X ����
		float GetCropX() const;

		// ��ȡ�ü�λ�� Y ����
		float GetCropY() const;

		// ��ȡ�ü�λ��
		Point GetCropPos() const;

		// ��ȡ�ü�����
		const Rect& GetCropRect() const;

		// ��ȡ ID2D1Bitmap ����
		ID2D1Bitmap * GetBitmap() const;

		// ��ջ���
		static void ClearCache();

	private:
		E2D_DISABLE_COPY(Image);

		// ���� Bitmap ��Դ
		static bool CacheBitmap(
			const std::wstring& file_name
		);

		// ���� Bitmap ��Դ
		static bool CacheBitmap(
			Resource& res
		);

		// ���� Bitmap
		void SetBitmap(
			ID2D1Bitmap * bitmap
		);

	private:
		Rect crop_rect_;
		ID2D1Bitmap * bitmap_;

		static std::map<size_t, ID2D1Bitmap*> bitmap_cache_;
	};


	class Node;

	// ����
	class Scene
		: public Ref
	{
	public:
		Scene();

		explicit Scene(
			Node * root
		);

		virtual ~Scene();

		// ���볡��
		virtual void OnEnter() {}

		// �˳�����
		virtual void OnExit() {}

		// ���³���
		virtual void OnUpdate(float dt) {}

		// ���ø��ڵ�
		void SetRoot(
			Node * root
		);

		// ��ȡ���ڵ�
		Node* GetRoot() const;

		// ��Ⱦ����
		void Draw();

		// �ַ������Ϣ
		virtual void Dispatch(
			const MouseEvent& e
		);

		// �ַ�������Ϣ
		virtual void Dispatch(
			const KeyEvent& e
		);

		// ����ת������
		void SetTransform(
			const D2D1::Matrix3x2F& matrix
		);

		// ��ȡת������
		const D2D1::Matrix3x2F& GetTransform() const;

	private:
		E2D_DISABLE_COPY(Scene);

	private:
		Node*				root_;
		D2D1::Matrix3x2F	transform_;
	};


	// ��ʱ����
	class Task
		: public Ref
	{
		friend class Node;

		typedef std::function<void()> Callback;

	public:
		explicit Task(
			const Callback& func,			/* ִ�к��� */
			const std::wstring& name = L""	/* �������� */
		);

		explicit Task(
			const Callback& func,			/* ִ�к��� */
			float delay,					/* ʱ�������룩 */
			int times = -1,					/* ִ�д������� -1 Ϊ����ִ�У� */
			const std::wstring& name = L""	/* �������� */
		);

		// ��������
		void Start();

		// ֹͣ����
		void Stop();

		// �����Ƿ�����ִ��
		bool IsRunning() const;

		// ��ȡ��������
		const std::wstring& GetName() const;

		// �����Ƿ����
		bool IsReady() const;

		// ִ������
		void Update();

		// ���ü�ʱ
		void ResetTime();

	private:
		bool		running_;
		bool		stopped_;
		int			run_times_;
		int			total_times_;
		std::wstring		name_;
		Duration	delay_;
		Time		last_time_;
		Callback	callback_;
		Node *		target_;
	};


	// ������Ϣ����ӿ�
	class KeyEventHandler
	{
	public:
		// ��������Ϣ
		virtual void Handle(KeyEvent e) = 0;
	};


	// �����Ϣ����ӿ�
	class MouseEventHandler
	{
	public:
		// ���������Ϣ
		virtual void Handle(MouseEvent e) = 0;
	};


	class Action;

	// �ڵ�
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

		// ��Ⱦ�ڵ�
		virtual void OnDraw() const {}

		// ���½ڵ�
		virtual void OnUpdate(float dt) {}

		// ��ȡ�ڵ���ʾ״̬
		bool IsVisible() const;

		// ��ȡ�ڵ�����
		const std::wstring& GetName() const;

		// ��ȡ�ڵ����Ƶ� Hash ֵ
		size_t GetHashName() const;

		// ��ȡ�ڵ��ͼ˳��
		int GetOrder() const;

		// ��ȡ�ڵ�����
		const Point& GetPosition() const;

		// ��ȡ�ڵ���
		float GetWidth() const;

		// ��ȡ�ڵ�߶�
		float GetHeight() const;

		// ��ȡ�ڵ��ȣ����������ţ�
		float GetRealWidth() const;

		// ��ȡ�ڵ�߶ȣ����������ţ�
		float GetRealHeight() const;

		// ��ȡ�ڵ��С�����������ţ�
		const Size& GetRealSize() const;

		// ��ȡ�ڵ��֧��
		float GetPivotX() const;

		// ��ȡ�ڵ��֧��
		float GetPivotY() const;

		// ��ȡ�ڵ��С
		Size GetSize() const;

		// ��ȡ�ڵ�������ű���
		float GetScaleX() const;

		// ��ȡ�ڵ��������ű���
		float GetScaleY() const;

		// ��ȡ�ڵ������б�Ƕ�
		float GetSkewX() const;

		// ��ȡ�ڵ�������б�Ƕ�
		float GetSkewY() const;

		// ��ȡ�ڵ���ת�Ƕ�
		float GetRotation() const;

		// ��ȡ��άת������
		const Transform& GetTransform() const;

		// ��ȡ�ڵ�͸����
		float GetOpacity() const;

		// ��ȡ��ʾ͸����
		float GetDisplayOpacity() const;

		// ��ȡ���ڵ�
		Node * GetParent() const;

		// ��ȡ�ڵ����ڳ���
		Scene * GetParentScene() const;

		// ���ýڵ��Ƿ���ʾ
		void SetVisible(
			bool value
		);

		// ���ýڵ�����
		void SetName(
			const std::wstring& name
		);

		// ���ýڵ������
		virtual void SetPositionX(
			float x
		);

		// ���ýڵ�������
		virtual void SetPositionY(
			float y
		);

		// ���ýڵ�����
		virtual void SetPosition(
			const Point & point
		);

		// ���ýڵ�����
		virtual void SetPosition(
			float x,
			float y
		);

		// �ƶ��ڵ�
		virtual void MoveBy(
			float x,
			float y
		);

		// �ƶ��ڵ�
		virtual void MoveBy(
			const Point & vector
		);

		// ���ýڵ��ͼ˳��
		// Ĭ��Ϊ 0
		virtual void SetOrder(
			int order
		);

		// ���ú������ű���
		// Ĭ��Ϊ 1.0
		virtual void SetScaleX(
			float scale_x
		);

		// �����������ű���
		// Ĭ��Ϊ 1.0
		virtual void SetScaleY(
			float scale_y
		);

		// �������ű���
		// Ĭ��Ϊ (1.0, 1.0)
		virtual void SetScale(
			float scale_x,
			float scale_y
		);

		// �������ű���
		// Ĭ��Ϊ 1.0
		virtual void SetScale(
			float scale
		);

		// ���ú�����б�Ƕ�
		// Ĭ��Ϊ 0
		virtual void SetSkewX(
			float skew_x
		);

		// ����������б�Ƕ�
		// Ĭ��Ϊ 0
		virtual void SetSkewY(
			float skew_y
		);

		// ������б�Ƕ�
		// Ĭ��Ϊ (0, 0)
		virtual void SetSkew(
			float skew_x,
			float skew_y
		);

		// ������ת�Ƕ�
		// Ĭ��Ϊ 0
		virtual void SetRotation(
			float rotation
		);

		// ����͸����
		// Ĭ��Ϊ 1.0, ��Χ [0, 1]
		virtual void SetOpacity(
			float opacity
		);

		// ����֧��ĺ���λ��
		// Ĭ��Ϊ 0, ��Χ [0, 1]
		virtual void SetPivotX(
			float pivot_x
		);

		// ����֧�������λ��
		// Ĭ��Ϊ 0, ��Χ [0, 1]
		virtual void SetPivotY(
			float pivot_y
		);

		// ����֧��λ��
		// Ĭ��Ϊ (0, 0), ��Χ [0, 1]
		virtual void SetPivot(
			float pivot_x,
			float pivot_y
		);

		// �޸Ľڵ���
		virtual void SetWidth(
			float width
		);

		// �޸Ľڵ�߶�
		virtual void SetHeight(
			float height
		);

		// �޸Ľڵ��С
		virtual void SetSize(
			float width,
			float height
		);

		// �޸Ľڵ��С
		virtual void SetSize(
			const Size & size
		);

		// ���ö�άת��
		virtual void SetTransform(
			const Transform& transform
		);

		// ���û�ر���Ⱦ����ü�
		virtual void SetClipEnabled(
			bool enabled
		);

		// ���ýڵ��Ե��ɫ
		virtual void SetBorderColor(
			const Color& color
		);

		// �жϵ��Ƿ��ڽڵ���
		bool ContainsPoint(
			const Point& point
		);

		// �ж��������Ƿ��ཻ
		bool Intersects(
			Node * node
		);

		// ����ӽڵ�
		void AddChild(
			Node * child,
			int order = 0		/* ��Ⱦ˳�� */
		);

		// ��Ӷ���ӽڵ�
		void AddChild(
			const Nodes& nodes,	/* �ڵ����� */
			int order = 0		/* ��Ⱦ˳�� */
		);

		// ��ȡ����������ͬ���ӽڵ�
		Nodes GetChildren(
			const std::wstring& name
		) const;

		// ��ȡ������ͬ���ӽڵ�
		Node* GetChild(
			const std::wstring& name
		) const;

		// ��ȡ�����ӽڵ�
		const Nodes& GetAllChildren() const;

		// ��ȡ�ӽڵ�����
		int GetChildrenCount() const;

		// �Ƴ��ӽڵ�
		bool RemoveChild(
			Node * child
		);

		// �Ƴ�����������ͬ���ӽڵ�
		void RemoveChildren(
			const std::wstring& child_name
		);

		// �Ƴ����нڵ�
		void RemoveAllChildren();

		// �Ӹ��ڵ��Ƴ�
		void RemoveFromParent();

		// ִ�ж���
		void RunAction(
			Action * action
		);

		// ��������
		void ResumeAction(
			const std::wstring& name
		);

		// ��ͣ����
		void PauseAction(
			const std::wstring& name
		);

		// ֹͣ����
		void StopAction(
			const std::wstring& name
		);

		// ����������ͣ����
		void ResumeAllActions();

		// ��ͣ���ж���
		void PauseAllActions();

		// ֹͣ���ж���
		void StopAllActions();

		// ��ȡ���ж���
		const Actions& GetAllActions() const;

		// �������
		void AddTask(
			Task * task
		);

		// ��������
		void StartTasks(
			const std::wstring& task_name
		);

		// ֹͣ����
		void StopTasks(
			const std::wstring& task_name
		);

		// �Ƴ�����
		void RemoveTasks(
			const std::wstring& task_name
		);

		// ������������
		void StartAllTasks();

		// ֹͣ��������
		void StopAllTasks();

		// �Ƴ���������
		void RemoveAllTasks();

		// ��ȡ��������
		const Tasks& GetAllTasks() const;

	protected:
		// �����ڵ�
		virtual void Visit();

		// �ַ������Ϣ
		virtual bool Dispatch(
			const MouseEvent& e,
			bool handled
		);

		// �ַ�������Ϣ
		virtual bool Dispatch(
			const KeyEvent& e,
			bool handled
		);

	private:
		E2D_DISABLE_COPY(Node);

		// ��Ⱦ�ڵ��Ե
		void DrawBorder();

		// ���ýڵ����ڳ���
		void SetParentScene(
			Scene * scene
		);

		// �����ӽڵ�
		void UpdateChildren(float dt);

		// ����ת������
		void UpdateTransform();

		// ���½ڵ�͸����
		void UpdateOpacity();

		// ���¶���
		void UpdateActions();

		// ��������
		void UpdateTasks();

		// ���½ڵ�ʱ��
		void UpdateTime();

	private:
		std::wstring				name_;
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


	// ����
	class Sprite
		: public Node
	{
	public:
		Sprite();

		explicit Sprite(
			Image * image
		);

		explicit Sprite(
			Resource& res
		);

		explicit Sprite(
			Resource& res,
			const Rect& crop_rect	/* �ü����� */
		);

		explicit Sprite(
			const std::wstring& file_name
		);

		explicit Sprite(
			const std::wstring& file_name,
			const Rect& crop_rect	/* �ü����� */
		);

		virtual ~Sprite();

		// ����ͼƬ�ļ�
		bool Load(
			Resource& res
		);

		// ����ͼƬ�ļ�
		bool Load(
			const std::wstring& file_name
		);

		// ����ͼƬ
		bool Load(
			Image * image
		);

		// ��ͼƬ�ü�Ϊ����
		void Crop(
			const Rect& crop_rect	/* �ü����� */
		);

		// ��ȡ Image ����
		Image * GetImage() const;

		// ��Ⱦ����
		virtual void OnDraw() const override;

	private:
		E2D_DISABLE_COPY(Sprite);

	private:
		Image* image_;
	};


	// �ı�
	class Text
		: public Node
	{
	public:
		// �ı����뷽ʽ
		enum class Align
		{
			Left,		/* ����� */
			Right,		/* �Ҷ��� */
			Center		/* ���ж��� */
		};

		// �ı���ʽ
		class Style
		{
		public:
			Color	color;				// ��ɫ
			Align	alignment;			// ���뷽ʽ
			bool	wrap;				// ���Զ�����
			float	wrap_width;			// �Զ����п��
			float	line_spacing;		// �м��
			bool	underline;			// �»���
			bool	strikethrough;		// ɾ����
			bool	outline;			// ��ʾ���
			Color	outline_color;		// �����ɫ
			float	outline_width;		// ����߿�
			Stroke	outline_stroke;		// ������ཻ��ʽ

		public:
			Style();

			Style(
				Color color,
				Align alignment			= Align::Left,
				bool wrap				= false,
				float wrap_width		= 0.f,
				float line_spacing		= 0.f,
				bool underline			= false,
				bool strikethrough		= false,
				bool outline			= true,
				Color outline_color		= Color(Color::Black, 0.5),
				float outline_width		= 1.f,
				Stroke outline_stroke = Stroke::Round
			);
		};

	public:
		Text();

		explicit Text(
			const std::wstring& text,			/* �������� */
			const Font& font	= Font(),		/* ���� */
			const Style& style	= Style()		/* �ı���ʽ */
		);

		virtual ~Text();

		// ��ȡ�ı�
		const std::wstring& GetText() const;

		// ��ȡ����
		const Font& GetFont() const;

		// ��ȡ�ı���ʽ
		const Style& GetStyle() const;

		// ��ȡ������
		const std::wstring& GetFontFamily() const;

		// ��ȡ��ǰ�ֺ�
		float GetFontSize() const;

		// ��ȡ��ǰ�����ϸֵ
		UINT GetFontWeight() const;

		// ��ȡ������ɫ
		const Color& GetColor() const;

		// ��ȡ�����ɫ
		const Color& GetOutlineColor() const;

		// ��ȡ����߿�
		float GetOutlineWidth() const;

		// ��ȡ������ཻ��ʽ
		Stroke GetOutlineStroke() const;

		// ��ȡ�ı���ʾ����
		int GetLineCount() const;

		// �Ƿ���б��
		bool IsItalic() const;

		// �Ƿ���ʾɾ����
		bool strikethrough() const;

		// �Ƿ���ʾ�»���
		bool underline() const;

		// �Ƿ���ʾ���
		bool outline() const;

		// �����ı�
		void SetText(
			const std::wstring& text
		);

		// �����ı���ʽ
		void SetStyle(
			const Style& style
		);

		// ��������
		void SetFont(
			const Font& font
		);

		// ����������
		void SetFontFamily(
			const std::wstring& family
		);

		// �����ֺţ�Ĭ��ֵΪ 22��
		void SetFontSize(
			float size
		);

		// ���������ϸֵ��Ĭ��ֵΪ Text::Font::Weight::Normal��
		void SetFontWeight(
			UINT weight
		);

		// ����������ɫ��Ĭ��ֵΪ Color::WHITE��
		void SetColor(
			Color color
		);

		// ��������б�壨Ĭ��ֵΪ false��
		void SetItalic(
			bool value
		);

		// �򿪻�ر��ı��Զ����У�Ĭ��Ϊ�رգ�
		void SetWrapEnabled(
			bool wrap
		);

		// �����ı��Զ����еĿ�ȣ�Ĭ��Ϊ 0��
		void SetWrapWidth(
			float wrap_width
		);

		// �����м�ࣨĬ��Ϊ 0��
		void SetLineSpacing(
			float line_spacing
		);

		// ���ö��뷽ʽ��Ĭ��Ϊ Align::Left��
		void SetAlignment(
			Align align
		);

		// �����»��ߣ�Ĭ��ֵΪ false��
		void SetUnderline(
			bool underline
		);

		// ����ɾ���ߣ�Ĭ��ֵΪ false��
		void SetStrikethrough(
			bool strikethrough
		);

		// �����Ƿ���ʾ���
		void SetOutline(
			bool outline
		);

		// ���������ɫ
		void SetOutlineColor(
			Color outline_color
		);

		// ��������߿�
		void SetOutlineWidth(
			float outline_width
		);

		// ����������ཻ��ʽ
		void SetOutlineStroke(
			Stroke outline_stroke
		);

		// ��Ⱦ����
		virtual void OnDraw() const override;

	private:
		E2D_DISABLE_COPY(Text);

		// �����Ű�����
		void Reset();

		// �������ָ�ʽ��
		void CreateFormat();

		// �������ֲ���
		void CreateLayout();

	private:
		std::wstring				text_;
		Font				font_;
		Style				style_;
		IDWriteTextFormat*	text_format_;
		IDWriteTextLayout*	text_layout_;
	};


	// ����
	class Canvas
		: public Node
	{
	public:
		Canvas(
			float width,
			float height
		);

		virtual ~Canvas();

		// ����������ɫ
		void SetLineColor(
			const Color& color
		);

		// ���������ɫ
		void SetFillColor(
			const Color& color
		);

		// �����������
		void SetStrokeWidth(
			float width
		);

		// ���������ཻ��ʽ
		void SetStrokeStyle(
			Stroke strokeStyle
		);

		// ��ȡ������ɫ
		Color GetLineColor() const;

		// ��ȡ�����ɫ
		Color GetFillColor() const;

		// ��ȡ�������
		float GetStrokeWidth() const;

		// ��ȡ�����ཻ��ʽ
		Stroke GetStrokeStyle() const;

		// ��ֱ��
		void DrawLine(
			const Point& begin,
			const Point& end
		);

		// ��Բ�α߿�
		void DrawCircle(
			const Point& center,
			float radius
		);

		// ����Բ�α߿�
		void DrawEllipse(
			const Point& center,
			float radius_x,
			float radius_y
		);

		// �����α߿�
		void DrawRect(
			const Rect& rect
		);

		// ��Բ�Ǿ��α߿�
		void DrawRoundedRect(
			const Rect& rect,
			float radius_x,
			float radius_y
		);

		// ���Բ��
		void FillCircle(
			const Point& center,
			float radius
		);

		// �����Բ��
		void FillEllipse(
			const Point& center,
			float radius_x,
			float radius_y
		);

		// ������
		void FillRect(
			const Rect& rect
		);

		// ���Բ�Ǿ���
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
