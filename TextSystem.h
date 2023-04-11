#ifndef TEXTSYSTEM_H
#define TEXTSYSTEM_H

enum TAGTYPES { DEFAULT_TAG };

namespace TXT
{
	/*========= Text Tag ==========*/
	class TextSystem
	{
	private:
		class TextTag
		{
		private:
			float dirY;
			float dirX;
			sf::Text text;
			float lifetime;
			float speed;

		public:
			//Constructors / Deconstructors
			TextTag(sf::Font& font, std::string text, float pos_x, float pos_y, float dir_x, float dir_y, sf::Color color, unsigned char_size, float lifetime, float speed)
				: lifetime(lifetime), speed(speed)
			{
				this->text.setFont(font);
				this->text.setPosition(pos_x, pos_y);
				this->text.setFillColor(color);
				this->text.setCharacterSize(char_size);
				this->text.setString(text);

				this->dirX = dir_x;
				this->dirY = dir_y;
				this->lifetime = lifetime;
				this->speed = speed;
			}

			TextTag(TextTag* tag, float pos_x, float pos_y, std::string text, const unsigned font_size)
			{
				this->text = tag->text;
				this->text.setString(text);
				this->text.setPosition(pos_x, pos_y);
				this->text.setCharacterSize(font_size);

				this->dirX = tag->dirX;
				this->dirY = tag->dirY;
				this->lifetime = tag->lifetime;
				this->speed = tag->speed;
			}

			~TextTag()
			{

			}

			//Accessors
			inline const bool isExpired() const { return this->lifetime <= 0.f; }

			//Functions
			void update(const float& dt)
			{
				if (this->lifetime > 0.f)
				{
					this->lifetime -= 100.f * dt;
					this->text.move(this->dirX * this->speed * dt, this->dirY * this->speed * dt);
				}
			}

			void render(sf::RenderTarget& target)
			{
				target.draw(this->text);
			}
		};

		sf::Font font;
		std::map<unsigned, TextTag*> tagTemplates;
		std::vector<TextTag*> tags;

		//Privát függvények
		void initFonts(std::string font_file);
		void initTagTemplates();

	public:
		//Constructor / Deconstructor
		TextSystem(std::string font_file);
		virtual ~TextSystem();

		//Függvények
		void addTextTag(const unsigned tag_type, const float pos_x, const float pos_y, const unsigned font_size);

		virtual void update(const float& dt);
		virtual void render(sf::RenderTarget& target);
	};
	/*========= Text Input ==========*/
#define DELETE_KEY 8
#define ENTER_KEY 13
#define ESCAPE_KEY 27

	class TextBox
	{
	private:
		sf::Text textbox;
		std::ostringstream text;
		bool isSelected;
		unsigned limit;

		//Privát függvények
		void inputLogic(int char_typed);
		void deleteLastChar();

	public:
		//Constructors / Deconstructors
		TextBox(sf::Vector2f pos, unsigned short char_size, sf::Font& font, sf::Color color, bool selected, const unsigned limit);
		virtual ~TextBox();
		
		//Segéd függvények
		const std::string getText() const;

		//Függvények
		void setSelected(bool selected);
		void typedOn(sf::Event* input);
		void render(sf::RenderTarget* target);
	};
}

#endif