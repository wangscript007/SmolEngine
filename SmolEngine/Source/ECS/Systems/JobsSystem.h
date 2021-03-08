#pragma once
#include "Core/Core.h"

#include "ECS/Components/Singletons/JobsSystemStateSComponent.h"

namespace SmolEngine
{
	enum class JobPriority : uint32_t
	{
		General,
		Rendering
	};

	class JobsSystem
	{
	public:

		template<typename F, typename...Args>
		static void Schedule(JobPriority priority, uint32_t job_group_id, F&& f, Args&&... args)
		{
			SubmitJob(priority, true, job_group_id, f, args...);
		}

		template<typename F, typename...Args>
		static void Assign(JobPriority priority, F&& f, Args&&... args)
		{
			SubmitJob(priority, false, 0, f, args...);
		}

		static void Complete(bool wait);

		// Getters

		static ThreadPool* GetThreadPool();

	private:

		template<typename F, typename...Args>
		static void SubmitJob(JobPriority priority, bool schedule, uint32_t job_group_id, F&& f, Args&&... args)
		{
			WorkerSpecialization spec = priority == JobPriority::General ? WorkerSpecialization::None : WorkerSpecialization::Rendering;
			JobsSystemStateSComponent* instance = JobsSystemStateSComponent::GetSingleton();
			std::function<decltype(f(args...))()> func = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
			auto task_ptr = std::make_shared<std::packaged_task<decltype(f(args...))()>>(func);
			std::function<void()> wrapper_func = [task_ptr]() {
				(*task_ptr)();
			};

			if (schedule)
			{
				bool id_found = false;
				for (auto& id : instance->JobGroupIDs)
				{
					if (id == job_group_id)
					{
						id_found = true;
						break;
					}
				}

				if (!id_found)
					instance->JobGroupIDs.push_back(job_group_id);

				JobDescription* job = new JobDescription();
				job->Func = wrapper_func;
				job->State = uint32_t(spec);
				job->Ptr = std::async(std::launch::deferred, [fut = std::move(task_ptr->get_future())]{ fut.wait(); });
				instance->SheduleTasks[job_group_id].push_back(job);
				return;
			}

			instance->ThreadPool.SubmitWork(spec, wrapper_func);
		}
	};
}