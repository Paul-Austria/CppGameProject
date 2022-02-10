namespace GameEngine {
	class Renderer
	{
	public:
		Renderer();
		~Renderer();

		static Renderer* GetInstance();


		void Init();
		void StartImGUI();
		void EndImGUI();


	private:
		static Renderer* instance;
	};
}