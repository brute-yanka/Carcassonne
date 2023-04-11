#ifndef GUI_H
#define GUI_H

enum button_states { BTN_IDLE = 0, BTN_HOVER, BTN_ACTIVE};

namespace GUI
{
	/*========= RESPONSIVE ==========*/
	const float p2pX(const float perc, const sf::VideoMode& vm);
	const float p2pY(const float perc, const sf::VideoMode& vm);
	const unsigned calcCharSize(const sf::VideoMode& vm, const unsigned modifier = 60);

	/*========= BUTTON ==========*/
	class Button
	{
	private:
		//Változók
		short unsigned buttonState;
		short unsigned id;
		
		sf::RectangleShape shape;
		sf::Font* font;
		sf::Text text;

		sf::Color textIdleColor;
		sf::Color textHoverColor;
		sf::Color textActiveColor;

		sf::Color idleColor;
		sf::Color hoverColor;
		sf::Color activeColor;

		sf::Color outlineIdleColor;
		sf::Color outlineHoverColor;
		sf::Color outlineActiveColor;

	public:
		//Constructors / Deconstructors
		Button(float x, float y, float width, float height,
			sf::Font* font, std::string text, unsigned character_size,
			sf::Color text_idle_color, sf::Color text_hover_color, sf::Color text_active_color,
			sf::Color idle_color, sf::Color hover_color, sf::Color active_color,
			sf::Color outline_idle_color = sf::Color::Transparent, sf::Color outline_hover_color = sf::Color::Transparent, sf::Color outline_active_color = sf::Color::Transparent,
			short unsigned id = 0
		);
		virtual ~Button();
		//Segéd Függvények
		const bool isPressed() const;
		const std::string getText() const;
		const short unsigned& getId() const;

		//Módosítók
		void setText(const std::string text);
		void setId(const short unsigned id);

		//Függvények
		void update(const sf::Vector2i& mousePosWindow);
		void render(sf::RenderTarget& target);

	};
	/*========= DROPDOWN LIST ==========*/
	class DropDownList
	{
	private:
		//Változók
		float keytime;
		float keytimeMax;
		sf::Font& font;
		GUI::Button* activeElement;
		std::vector<GUI::Button*> list;
		bool showList;

	public:
		//Constructors / Deconstructors
		DropDownList(float x, float y, float width, float height, sf::Font& font, unsigned character_size, std::string list[], unsigned nrOfElements, unsigned default_index = 0);
		virtual ~DropDownList();

		//Segéd Függvények
		const bool getKeyTime();
		const unsigned short& getActiveElementId() const;

		//Függvények
		void updateKeyTime(const float& dt);
		void update(const sf::Vector2i& mousePosWindow, const float& dt);
		void render(sf::RenderTarget& target);
	};
}

#endif