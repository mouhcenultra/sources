#include "obj_dir/Vtb_compute_tile__Syms.h"
#include "obj_dir/Vtb_compute_tile__Dpi.h"

#include <VerilatedToplevel.h>
#include <VerilatedControl.h>

#include <ctime>
#include <cstdlib>

using namespace simutilVerilator;

VERILATED_TOPLEVEL(tb_compute_tile,clk, rst)

int main(int argc, char *argv[])
{
    tb_compute_tile ct("TOP");

    VerilatedControl &simctrl = VerilatedControl::instance();
    simctrl.init(ct, argc, argv);

    simctrl.addMemory("TOP.tb_compute_tile.u_compute_tile.u_ram.sp_ram.gen_sram_sp_impl.u_impl");
    simctrl.run();

    return 0;
}
