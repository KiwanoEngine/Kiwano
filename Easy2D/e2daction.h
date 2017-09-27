#pragma once
#include <e2dbase.h>

namespace easy2d
{
	class Action :
		public Object
	{
		friend class Sprite;
		friend class ActionManager;
		friend class ActionTwo;
		friend class ActionNeverStop;
		friend class ActionSequence;
	public:
		Action();
		virtual ~Action();

		bool isRunning();
		void start();
		void resume();
		void pause();
		void stop();
		void setInterval(UINT ms);
		virtual Action * copy() = 0;
		virtual Action * reverse() const;

	protected:
		bool			m_bRunning;
		bool			m_bStop;
		Sprite *		m_pParent;
		UINT			m_nMilliSeconds;
		LARGE_INTEGER	m_nLast;
		LARGE_INTEGER	m_nAnimationInterval;

	protected:
		virtual void _init() = 0;
		virtual bool _exec(LARGE_INTEGER nNow) = 0;
		virtual void _reset() = 0;
	};


	class Animation :
		public Action
	{
	public:
		Animation(float duration);
		virtual ~Animation();

	protected:
		UINT	m_nDuration;
		UINT	m_nTotalDuration;

	protected:
		virtual void _init() override;
		virtual bool _exec(LARGE_INTEGER nNow) override;
		virtual void _reset() override;
	};


	class ActionMoveBy :
		public Animation
	{
	public:
		ActionMoveBy(float duration, CVector vec);
		virtual ~ActionMoveBy();

		virtual ActionMoveBy * copy() override;
		virtual ActionMoveBy * reverse() const override;

	protected:
		CPoint	m_BeginPos;
		CVector	m_MoveVector;

	protected:
		virtual void _init() override;
		virtual bool _exec(LARGE_INTEGER nNow) override;
		virtual void _reset() override;
	};


	class ActionMoveTo :
		public ActionMoveBy
	{
	public:
		ActionMoveTo(float duration, CPoint pos);
		virtual ~ActionMoveTo();

		virtual ActionMoveTo * copy() override;

	protected:
		CPoint m_EndPos;

	protected:
		virtual void _init() override;
		virtual void _reset() override;
	};


	class ActionScaleBy :
		public Animation
	{
	public:
		ActionScaleBy(float duration, float scaleX, float scaleY);
		virtual ~ActionScaleBy();

		virtual ActionScaleBy * copy() override;
		virtual ActionScaleBy * reverse() const override;

	protected:
		float	m_nBeginScaleX;
		float	m_nBeginScaleY;
		float	m_nVariationX;
		float	m_nVariationY;

	protected:
		virtual void _init() override;
		virtual bool _exec(LARGE_INTEGER nNow) override;
		virtual void _reset() override;
	};


	class ActionScaleTo :
		public ActionScaleBy
	{
	public:
		ActionScaleTo(float duration, float scaleX, float scaleY);
		virtual ~ActionScaleTo();

		virtual ActionScaleTo * copy() override;

	protected:
		float	m_nEndScaleX;
		float	m_nEndScaleY;

	protected:
		virtual void _init() override;
		virtual void _reset() override;
	};


	class ActionOpacityBy :
		public Animation
	{
	public:
		ActionOpacityBy(float duration, float opacity);
		virtual ~ActionOpacityBy();

		virtual ActionOpacityBy * copy() override;
		virtual ActionOpacityBy * reverse() const override;

	protected:
		float m_nBeginVal;
		float m_nVariation;

	protected:
		virtual void _init() override;
		virtual bool _exec(LARGE_INTEGER nNow) override;
		virtual void _reset() override;
	};


	class ActionOpacityTo :
		public ActionOpacityBy
	{
	public:
		ActionOpacityTo(float duration, float opacity);
		virtual ~ActionOpacityTo();

		virtual ActionOpacityTo * copy() override;

	protected:
		float m_nEndVal;

	protected:
		virtual void _init() override;
		virtual void _reset() override;
	};


	class ActionFadeIn :
		public ActionOpacityTo
	{
	public:
		ActionFadeIn(float duration) : ActionOpacityTo(duration, 1) {}
	};


	class ActionFadeOut :
		public ActionOpacityTo
	{
	public:
		ActionFadeOut(float duration) : ActionOpacityTo(duration, 0) {}
	};


	class ActionTwo :
		public Action
	{
	public:
		ActionTwo(Action * actionFirst, Action * actionSecond);
		virtual ~ActionTwo();

		virtual ActionTwo * copy() override;
		virtual ActionTwo * reverse() const override;

	protected:
		Action * m_FirstAction;
		Action * m_SecondAction;

	protected:
		virtual void _init() override;
		virtual bool _exec(LARGE_INTEGER nNow) override;
		virtual void _reset() override;
	};


	class ActionSequence :
		public Action
	{
	public:
		ActionSequence(int number, Action * action1, ...);
		virtual ~ActionSequence();

		virtual ActionSequence * copy() override;
		virtual ActionSequence * reverse() const override;

	protected:
		UINT					m_nActionIndex;
		std::vector<Action*>	m_vActions;

	protected:
		virtual void _init() override;
		virtual bool _exec(LARGE_INTEGER nNow) override;
		virtual void _reset() override;
	};


	class ActionDelay :
		public Action
	{
	public:
		ActionDelay(float duration);
		virtual ~ActionDelay();

		virtual ActionDelay * copy() override;

	protected:
		virtual void _init() override;
		virtual bool _exec(LARGE_INTEGER nNow) override;
		virtual void _reset() override;
	};


	class ActionNeverStop :
		public Action
	{
	public:
		ActionNeverStop(Action * action);
		virtual ~ActionNeverStop();

		virtual ActionNeverStop * copy() override;

	protected:
		Action * m_Action;

	protected:
		virtual void _init() override;
		virtual bool _exec(LARGE_INTEGER nNow) override;
		virtual void _reset() override;
	};


	class ActionFrames :
		public Action
	{
	public:
		ActionFrames();
		~ActionFrames();

		void addFrame(Image * frame);
		virtual ActionFrames * copy() override;
		virtual ActionFrames * reverse() const override;

	protected:
		UINT				m_nFrameIndex;
		std::vector<Image*> m_vFrames;

	protected:
		virtual void _init() override;
		virtual bool _exec(LARGE_INTEGER nNow) override;
		virtual void _reset() override;
	};


	class ActionCallback :
		public Action
	{
	public:
		ActionCallback(const std::function<void()>& callback);
		~ActionCallback();

		virtual ActionCallback * copy() override;

	protected:
		std::function<void()> m_Callback;

	protected:
		virtual void _init() override;
		virtual bool _exec(LARGE_INTEGER nNow) override;
		virtual void _reset() override;
	};

}	// End of easy2d namespace