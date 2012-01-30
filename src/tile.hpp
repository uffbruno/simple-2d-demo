class tile: public character {
	public:
		tile();
		~tile();
		
		void impl_draw();
		state impl_update();
};