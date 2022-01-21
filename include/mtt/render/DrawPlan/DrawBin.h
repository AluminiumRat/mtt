#pragma once

#include <memory>
#include <vector>

#include <mtt/render/DrawPlan/AbstractAction.h>
#include <mtt/render/DrawPlan/ActionMemoryPool.h>

namespace mtt
{
	class DrawBin
	{
	public:
		DrawBin(ActionMemoryPool& memoryPool);
		DrawBin(const DrawBin&) = delete;
		DrawBin& operator = (const DrawBin&) = delete;
		virtual ~DrawBin() noexcept = default;

		template<typename ActionType, typename... Args>
		inline ActionType& createAction(float priority, Args&... args);

		inline bool empty() const noexcept;
		inline void clear() noexcept;

		void scheduleDraw(DrawContext& drawContext);

	private:
		struct ActionRecord
		{
			ActionPtr action;
			float priority;
	
			inline ActionRecord(ActionPtr&& theAction,
													float thePriority);
			inline ActionRecord(ActionRecord&& other);
			ActionRecord(const ActionRecord&) = delete;
			ActionRecord& operator = (const ActionRecord&) = delete;
			inline ActionRecord& operator = (ActionRecord&& other);
			~ActionRecord() = default;
		};

		using Queue = std::vector<ActionRecord>;
		Queue _queue;

		ActionMemoryPool& _memoryPool;
	};

	inline DrawBin::ActionRecord::ActionRecord(	ActionPtr&& theAction,
																							float thePriority) :
		action(std::move(theAction)),
		priority(thePriority)
	{
	}

	inline DrawBin::ActionRecord::ActionRecord(ActionRecord&& other) :
		action(std::move(other.action)),
		priority(other.priority)
	{
	}

	inline DrawBin::ActionRecord& DrawBin::ActionRecord::operator = (
																													ActionRecord&& other)
	{
		action = std::move(other.action);
		priority = other.priority;
		return *this;
	}

	template<typename ActionType, typename... Args>
	inline ActionType& DrawBin::createAction(float priority, Args&... args)
	{
		ActionPtr newAction = _memoryPool.emplace<ActionType>(args...);
		ActionType& newActionRef = static_cast<ActionType&>(*newAction);
		_queue.emplace_back(std::move(newAction), priority);
		return newActionRef;
	}

	inline bool DrawBin::empty() const noexcept
	{
		return _queue.empty();
	}

	inline void DrawBin::clear() noexcept
	{
		_queue.clear();
	}
}