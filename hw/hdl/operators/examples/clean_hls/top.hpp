#include <hls_stream.h>
#include <stdint.h>
#include <ap_int.h>

//===========================================================================
// Design Dimensioning

template<unsigned D>
struct net_axis {
	ap_uint<D>		data;
	ap_uint<D/8>	keep;
	ap_uint<1>		last;

    net_axis<D>()
        : data(0), keep(0), last(0) {}

    net_axis<D>(ap_uint<D> data, ap_uint<D/8> keep, ap_uint<1> last)
        : data(data), keep(keep), last(last) {}
};

using input_t	= net_axis<512>;			// Input Tuple
using output_t	= net_axis<32>;				// Output: Wrapped float


//===========================================================================
void top(
	hls::stream<input_t>	&s_axis_data,
	hls::stream<output_t>	&m_axis_card
) {
// #pragma HLS dataflow

    const int n_iter = 1024
    for (int i = 0; i < n_iter; i++) {
        input_t tmp_in = s_axis_data.read();
        ap_uint<512> tmp_in_data = tmp_in.data;

        ap_uint<1> last = 0b0;
        if (i == n_iter - 1) {
            last = 0b1;
        }
        output_t tmp_out(tmp_in_data.range(31, 0), 0b1111, last);
        
        m_axis_card.write(tmp_out);
    }

} // top
