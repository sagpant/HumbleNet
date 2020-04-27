//
//  Menu.h
//  SimpleGameFramework
//
//  Created by Edward Rudd on 5/15/15.
//
//

#pragma once

#include <utility>
#include <vector>

#include "engine/Scene.h"
#include "engine/BakedFont.h"

class Menu : public Scene {
public:
	class BaseItem {
	protected:
		std::string m_label;
		bool m_disabled = false;
	public:
		explicit BaseItem(std::string label) : m_label( std::move( label )) {}

		virtual ~BaseItem() = default;

		virtual std::string label() { return m_label; }

		virtual void enter(Scene& scene) {}

		virtual void exit(Scene& scene) {}

		virtual void update(Scene& scene, float delta) {}

		virtual bool action_select(Scene& scene) { return false; }

		virtual bool action_left(Scene& scene) { return false; }

		virtual bool action_right(Scene& scene) { return false; }

		bool disabled() const { return m_disabled; }

		void setDisabled(bool disabled) { m_disabled = disabled; }
	};

protected:
	BakedFontRef m_font;

	uint16_t m_current_item;

	std::vector<std::unique_ptr<BaseItem>> m_menu_items;
	std::string m_previous_scene;
protected:
	void onShow(const std::string& previous_name, const SceneRef previous) override;

	template<class C, typename ...Args>
	decltype(m_menu_items)::size_type add(Args&& ...args)
	{
		m_menu_items.emplace_back( new C( std::forward<Args>( args )... ));
		return m_menu_items.size() - 1;
	}

	template<class C>
	C* item(decltype(m_menu_items)::size_type pos)
	{
		if (pos < m_menu_items.size()) {
			auto& o = m_menu_items.at( pos );
			auto ptr = o.get();
			return dynamic_cast<C*>(ptr);
		}
		return nullptr;
	}

	void handleBack();

public:
	explicit Menu(Renderer& rend);

	void update(const InputManager& input, float delta) override;

	void render() override;
};