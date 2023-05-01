struct Vertex {
	float4 m_position: SV_POSITION;
	float4 m_color: COLOR0;
};

struct Interpolant {
	float4 m_position: SV_POSITION;
	float4 m_color: COLOR0;
};

Interpolant main(Vertex vertex) {
	Interpolant interpolant;
	interpolant.m_position = vertex.m_position;
	interpolant.m_color = vertex.m_color;

	return interpolant;
}