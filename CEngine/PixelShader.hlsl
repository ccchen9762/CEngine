struct Interpolant {
	float4 m_position: SV_POSITION;
	float4 m_color: COLOR0;
};

struct Pixel {
	float4 m_color: SV_TARGET;
};

Pixel main(Interpolant interpolant) {
	Pixel pixel;
	pixel.m_color = interpolant.m_color;

	return pixel;
}