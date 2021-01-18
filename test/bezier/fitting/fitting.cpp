#include <gtkmm.h>
#include <gtkmm/drawingarea.h>
#include <cairomm/cairomm.h>
#include <gtkmm/spinbutton.h>

#include <memory>
#include <vector>

#include <glm/vec2.hpp>
#include <fmt/format.h>
#include <ez/geo/BezierFunctions.hpp>
#include <ez/geo/BPath.hpp>

void drawCubic(const Cairo::RefPtr<Cairo::Context>& ctx, const std::array<glm::dvec2, 4>& p) {
	ctx->move_to(p[0].x, p[0].y);

	ctx->curve_to(p[1].x, p[1].y, p[2].x, p[2].y, p[3].x, p[3].y);
	ctx->stroke();
}

int main(int argc, char ** argv) {
	auto app =
		Gtk::Application::create(argc, argv,
			"org.gtkmm.examples.base");

	std::unique_ptr<Gtk::Window> window{ new Gtk::Window{} };
	window->set_default_size(800, 600);

	Gtk::DrawingArea drawingArea;
	Gtk::HBox hbox;
	Gtk::VBox vbox;

	vbox.set_size_request(160, -1);

	auto updateFunc = [&]() {
		drawingArea.queue_draw();
	};

	std::array<Gtk::SpinButton, 3> spins;

	hbox.pack_start(drawingArea);
	hbox.pack_start(vbox, Gtk::PACK_SHRINK);
	for (auto& spin : spins) {
		vbox.pack_start(spin, Gtk::PACK_SHRINK);
		auto discard = spin.signal_value_changed().connect(updateFunc);
		spin.set_range(0, 1000);
		spin.set_digits(6);
	}

	spins[0].set_value(0.05);
	spins[1].set_value(0.9999);

	spins[2].set_digits(0);
	spins[2].set_value(45);
	spins[2].set_increments(1, 5);

    auto drawFunc = [&](const Cairo::RefPtr<Cairo::Context>& ctx) -> bool {
		glm::dvec2 pos, size;
		ctx->get_clip_extents(pos.x, pos.y, size.x, size.y);

		ez::Bezier::alph = spins[0].get_value();
		ez::Bezier::gamma = spins[1].get_value();
		ez::Bezier::itermin = static_cast<std::size_t>(spins[2].get_value());

		std::array<double, 7> tdata{
			1.0 / 8.0,
			2.0 / 8.0,
			3.0 / 8.0,

			4.0 / 8.0,
			5.0 / 8.0,
			6.0 / 8.0,
			7.0 / 8.0
		};
		std::array<double, 7> vdata{
			0.1,
			0.2,
			0.25,

			0.26,
			0.27,
			0.3,
			0.5,
		};

		std::array<double, 4> curve;

		ez::Bezier::fitCubic(vdata.begin(), vdata.begin() + 7, tdata.begin(), tdata.begin() + 7, curve.begin());

		ctx->set_source_rgb(0, 0, 0);

		for (int i = 0; i < tdata.size(); ++i) {
			double x = tdata[i] * size.x + pos.x;
			double y = vdata[i] * size.y + pos.y;
			ctx->arc(x, y, 4.0, 0.0, ez::tau());
			ctx->fill();
		}

		ctx->set_source_rgb(0, 0, 1);

		ctx->move_to(pos.x, curve[0] * size.y + pos.y);
		for (double t = 0.01; t < 1.0; t += 0.01) {
			double val = ez::Bezier::interpolate(curve[0], curve[1], curve[2], curve[3], t);

			ctx->line_to(t * size.x + pos.x, val * size.y + pos.y);
		}
		ctx->stroke();

		ctx->set_source_rgb(1, 0, 0);
		for (int i = 0; i < curve.size(); ++i) {
			ctx->arc((i / 3.0) * size.x + pos.x, curve[i] * size.y + pos.y, 4, 0.0, ez::tau());
			ctx->fill();
		}

		double error = 0.0;
		for (int i = 0; i < tdata.size(); ++i) {
			double t = tdata[i];
			double v = vdata[i];

			double est = ez::Bezier::interpolate(curve[0], curve[1], curve[2], curve[3], t);

			double tmp = (v - est);
			error += tmp * tmp;
		}
		fmt::print("The net error is: {}\n", error);

        return true;
    };
    auto discard = drawingArea.signal_draw().connect(drawFunc);

    window->add(hbox);
    window->show_all_children();
    window->show();

    return app->run(*window);
}
