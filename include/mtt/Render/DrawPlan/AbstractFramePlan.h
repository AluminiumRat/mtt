#pragma once

#include <vector>

#include <mtt/Render/DrawPlan/ActionMemoryPool.h>
#include <mtt/Render/DrawPlan/DrawBin.h>
#include <mtt/Render/DrawPlan/StageIndex.h>

namespace mtt
{
	class AbstractFrame;
	class AbstractFrameBuilder;

	class AbstractFramePlan
	{
	protected:
		AbstractFramePlan(AbstractFrame& theFrame);

	public:
		AbstractFramePlan(const AbstractFramePlan&) = delete;
		AbstractFramePlan& operator = (const AbstractFramePlan&) = delete;
		virtual ~AbstractFramePlan() noexcept = default;

		inline DrawBin* getBin(StageIndex stage) noexcept;
		inline const DrawBin* getBin(StageIndex stage) const noexcept;

		inline AbstractFrame& frame() const noexcept;
		inline AbstractFrameBuilder& frameBuilder() const noexcept;

	protected:
		inline ActionMemoryPool& memoryPool() noexcept;
		inline const ActionMemoryPool& memoryPool() const noexcept;
		void registerBin(DrawBin& bin, StageIndex stage);

	private:
		ActionMemoryPool _memoryPool;
		AbstractFrame& _frame;
		AbstractFrameBuilder& _frameBuilder;

		std::vector<DrawBin*> _bins;
	};

	inline DrawBin* AbstractFramePlan::getBin(StageIndex stage) noexcept
	{
		if(_bins.size() <= stage.index()) return nullptr;
		return _bins[stage.index()];
	}

	inline const DrawBin* AbstractFramePlan::getBin(StageIndex stage) const noexcept
	{
		if (_bins.size() <= stage.index()) return nullptr;
		return _bins[stage.index()];
	}

	inline AbstractFrame& AbstractFramePlan::frame() const noexcept
	{
		return _frame;
	}

	inline AbstractFrameBuilder& AbstractFramePlan::frameBuilder() const noexcept
	{
		return _frameBuilder;
	}

	inline ActionMemoryPool& AbstractFramePlan::memoryPool() noexcept
	{
		return _memoryPool;
	}

	inline const ActionMemoryPool& AbstractFramePlan::memoryPool() const noexcept
	{
		return _memoryPool;
	}
}