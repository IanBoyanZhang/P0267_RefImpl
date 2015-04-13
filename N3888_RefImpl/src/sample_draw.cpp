#include "sample_draw.h"
#include "xio2dhelpers.h"
#include <cmath>
#include <random>
#include <vector>
#include <algorithm>
#include <string>
#include <sstream>
#include <chrono>

using namespace std;
using namespace std::chrono;
using namespace std::experimental::io2d;

namespace {
	static const double pi = 3.14159265358979323846264338327950288;
}

// Declaration

void draw_radial_circles(display_surface& rs);

// Declaration
wostream& operator<<(wostream& os, const vector_2d& pt);
// Declaration
vector<vector<int>> init_sort_steps(int count, unsigned long mtSeed = 1009UL);
// Declaration
void draw_hello_world(surface& rs);
// Declaration
void draw_test_compositing_operators(surface& rs, double elapsedTimeInMilliseconds, compositing_operator secondRectCompOp, compositing_operator firstRectCompOp = compositing_operator::over, bool strokePaths = false, const rgba_color& backgroundColor = rgba_color::transparent_black(), const rgba_color& firstColor = rgba_color::red() * 0.8, const rgba_color& secondColor = rgba_color::teal() * 0.4, bool clipToRects = false, bool clipToTriangle = false);
// Declaration
void draw_sort_visualization_immediate(surface& rs, double elapsedTimeInMilliseconds);
// Declaration
void draw_sort_visualization(surface& rs, double elapsedTimeInMilliseconds);

// Drawing entry point.
void sample_draw::operator()(display_surface& rs) {
	//draw_radial_circles(rs);

	static auto previousTime = steady_clock::now();
	auto currentTime = steady_clock::now();
	auto elapsedTime = currentTime - previousTime;
	previousTime = currentTime;
	draw_sort_visualization_immediate(rs, duration_cast<microseconds>(elapsedTime).count() / 1000.0);
	//draw_test_compositing_operators(rs, elapsedTimeInMilliseconds, compositing_operator::over);
}

//rgba_color interpolated_color(const radial_brush_factory& f, double t) {
//	while (t > 1.0) {
//		t -= 1.0;
//	}
//	while (t < 0.0) {
//		t += 1.0;
//	}
//	auto lowColorValue = rgba_color{ };
//	auto lowColorOffset = -1.0;
//	auto lowColorIndex = 0U;
//	auto highColorValue = rgba_color{ };
//	auto highColorOffset = -1.0;
//	auto highColorIndex = 0U;
//	auto lastOffset = -1.0;
//	auto sameOffset = true;
//	for (auto i = 0U; i < f.color_stop_count(); i++) {
//		auto stop = f.color_stop(i);
//		if (lastOffset < 0.0) {
//			lastOffset = get<0>(stop);
//		}
//		else {
//			if (lastOffset != get<0>(stop)) {
//				sameOffset = false;
//			}
//		}
//		if (lowColorOffset < 0.0) {
//			if (get<0>(stop) <= t) {
//				lowColorIndex = i;
//				lowColorOffset = get<0>(stop);
//				lowColorValue = get<1>(stop);
//			}
//		}
//		else {
//			if (get<0>(stop) <= t && get<0>(stop) > lowColorOffset) {
//				lowColorIndex = i;
//				lowColorOffset = get<0>(stop);
//				lowColorValue = get<1>(stop);
//			}
//		}
//	}
//	return rgba_color{ };
//}

rgba_color test_draw_circle(display_surface& /*rs*/, const vector_2d& coords, const radial_brush_factory& f) {
	// Can get t for s_fn via a simple magnitude. Determine the rotation angle of coords around center0, determine the point on circle 0 with that angle then the point on cirle 1 with that angle then get the distances from circle 0 pt to coords and from circle 0 pt to circle 1 pt and t = (dist(circ0, coords) / dist(circ0, circ1).
	auto circles = f.radial_circles();
	auto center0 = get<0>(circles);
	auto radius0 = get<1>(circles);
	auto center1 = get<2>(circles);
	auto radius1 = get<3>(circles);
	auto coordsAtZeroOrigin = coords - center0;
	auto angle = (coordsAtZeroOrigin.x() == coordsAtZeroOrigin.y() == 0.0) ? 0.0 : atan2(coordsAtZeroOrigin.y(), coordsAtZeroOrigin.x());
	auto circ0 = _Rotate_point_absolute_angle(center0, radius0, angle);
	auto circ1 = _Rotate_point_absolute_angle(center1, radius1, angle);
	auto dist_fn = [](const vector_2d& from, const vector_2d& to) -> double { return (to - from).length(); };
	if (_Almost_equal_relative(dist_fn(circ0, circ1), 0.0)) {
		return{ };
	}
	auto t = dist_fn(circ0, coords) / dist_fn(circ0, circ1);
	auto s_fn = [](double t) -> double { return (t - 0.0) / (1.0 - 0.0); };
	auto x_fn = [&circles](double s) -> double { auto x0 = get<0>(circles).x(); return x0 + s * (get<2>(circles).x() - x0); };
	auto y_fn = [&circles](double s) -> double { auto y0 = get<0>(circles).y(); return y0 + s * (get<2>(circles).y() - y0); };
	auto r_fn = [&circles](double s) -> double { auto r0 = get<1>(circles); return r0 + s * (get<3>(circles) - r0); };
	auto s = s_fn(t);
	auto x = x_fn(s);
	auto y = y_fn(s);
	auto r = r_fn(s);
	(void)x;
	(void)y;
	(void)r;
	return{ };
}

void draw_radial_circles(display_surface& rs) {
	// Clear to background color.
	rs.paint(rgba_color::cornflower_blue());

	auto radialFactory = radial_brush_factory();
	radialFactory.add_color_stop(0.0, rgba_color::white());
	//radialFactory.add_color_stop(0.25, rgba_color::red());
	//radialFactory.add_color_stop(0.5, rgba_color::green());
	//radialFactory.add_color_stop(0.75, rgba_color::blue());
	//radialFactory.add_color_stop(1.0, rgba_color::black());
	radialFactory.add_color_stop(1.0, rgba_color::blue());
	//radialFactory.radial_circles({ 400.0, 200.0 }, 100.0, { 600.0, 200.0 }, 50.0);
	auto radialBrush = brush(radialFactory);
	//radialFactory.radial_circles({ 115.2, 102.4 }, 25.6, { 102.4, 102.4 }, 128.0);
	//radialBrush = brush(radialFactory);
	//radialBrush.extend(extend::pad);
	//rs.immediate().clear();
	//rs.immediate().arc({ 128.0, 128.0 }, 76.8, 0.0, 2 * pi);
	////rs.immediate().rectangle({ 0.0, 0.0, 250.0, 250.0 });
	//rs.fill_immediate(radialBrush);

	//radialFactory.radial_circles({ 400.0, 200.0 }, 100.0, { 600.0, 200.0 }, 100.0);
	//auto color = test_draw_circle(rs, { 400.0, 200.0 }, radialFactory);
	auto linearFactory = linear_brush_factory();
	linearFactory.begin_point({ 200.0, 0.0 });
	linearFactory.end_point({ 601.0, 0.0 });
	linearFactory.add_color_stop(0.0, rgba_color::white());
	linearFactory.add_color_stop(0.25, rgba_color::red());
	linearFactory.add_color_stop(0.5, rgba_color::lime());
	linearFactory.add_color_stop(0.6, rgba_color::red());
	linearFactory.add_color_stop(0.5, rgba_color::blue());
	linearFactory.add_color_stop(1.0, rgba_color::white());

	rs.immediate().clear();
	rs.immediate().rectangle({ { 200.0, 280.0 }, { 602.0, 520.0 } });
	rs.fill_immediate(brush(linearFactory));

	//radialFactory.radial_circles({ 600.0, 200.0 }, 100.0, { 400.0, 200.0 }, 100.0);
	//rs.immediate().clear();
	//rs.immediate().rectangle({ { 280.0, 80.0 }, { 720.0, 320.0 } });
	//radialBrush = brush(radialFactory);
	//radialBrush.extend(extend::pad);
	//rs.fill_immediate(radialBrush);

	//radialFactory.radial_circles({ 400.0, 450.0 }, 100.0, { 600.0, 450.0 }, 100.0);
	//rs.immediate().clear();
	//rs.immediate().rectangle({ { 280.0, 330.0 }, { 720.0, 570.0 } });
	//radialBrush = brush(radialFactory);
	//radialBrush.extend(extend::none);
	//rs.fill_immediate(radialBrush);

	//radialFactory.radial_circles({ 1000.0, 200.0 }, 100.0, { 1000.0, 200.0 }, 20.0);
	//rs.immediate().clear();
	//rs.immediate().rectangle({ { 880.0, 80.0 }, { 1120.0, 320.0 } });
	//radialBrush = brush(radialFactory);
	//radialBrush.extend(extend::reflect);
	//rs.fill_immediate(radialBrush);

	//radialFactory.radial_circles({ 1000.0, 450.0 }, 20.0, { 1000.0, 450.0 }, 100.0);
	//rs.immediate().clear();
	//rs.immediate().rectangle({ { 880.0, 330.0 }, { 1120.0, 570.0 } });
	//radialBrush = brush(radialFactory);
	//radialBrush.extend(extend::repeat);
	//rs.fill_immediate(radialBrush);

	//radialFactory.radial_circles({ 105.0, 100.0 }, 0.0, { 200.0, 100.0 }, 100.0);
	//rs.immediate().clear();
	//rs.immediate().rectangle({ { 0.0, 0.0 }, { 520.0, 200.0 } });
	//radialBrush = brush(radialFactory);
	//radialBrush.extend(extend::repeat);
	//rs.fill_immediate(radialBrush);


	//radialFactory.radial_circles({ 200.0, 200.0 }, 100.0, { 400.0, 200.0 }, 100.0);
	//rs.immediate().clear();
	//rs.immediate().rectangle({ { 80.0, 80.0 }, { 520.0, 320.0 } });
	//radialBrush = brush(radialFactory);
	//radialBrush.extend(extend::none);
	//rs.fill_immediate(radialBrush);

	//radialFactory.radial_circles({ 200.0, 450.0 }, 100.0, { 400.0, 450.0 }, 100.0);
	//rs.immediate().clear();
	//rs.immediate().rectangle({ { 80.0, 330.0 }, { 520.0, 570.0 } });
	//radialBrush = brush(radialFactory);
	//radialBrush.extend(extend::pad);
	//rs.fill_immediate(radialBrush);

	////radialFactory.radial_circles({ 200.0, 450.0 }, 100.0, { 400.0, 450.0 }, 100.0);
	////rs.immediate().clear();
	////rs.immediate().rectangle({ { 80.0, 330.0 }, { 520.0, 570.0 } });
	////radialBrush = brush(radialFactory);
	////radialBrush.extend(extend::pad);
	////rs.fill_immediate(radialBrush);

	//radialFactory.radial_circles({ 700.0, 200.0 }, 100.0, { 900.0, 200.0 }, 100.0);
	//rs.immediate().clear();
	//rs.immediate().rectangle({ { 580.0, 80.0 }, { 1020.0, 320.0 } });
	//radialBrush = brush(radialFactory);
	//radialBrush.extend(extend::reflect);
	//rs.fill_immediate(radialBrush);

	//radialFactory.radial_circles({ 700.0, 450.0 }, 100.0, { 900.0, 450.0 }, 100.0);
	//rs.immediate().clear();
	//rs.immediate().rectangle({ { 580.0, 330.0 }, { 1020.0, 570.0 } });
	//radialBrush = brush(radialFactory);
	//radialBrush.extend(extend::repeat);
	//rs.fill_immediate(radialBrush);


}

wostream& operator<<(wostream& os, const vector_2d& pt) {
	os << L"(" << pt.x() << L"," << pt.y() << L")";
	return os;
}

vector<vector<int>> init_sort_steps(int count, unsigned long mtSeed) {
	vector<vector<int>> result;
	result.push_back([count, mtSeed]() {
		vector<int> init;
		for (int i = 0; i < count; ++i) {
			init.push_back(i);
		}
		mt19937 rng(mtSeed);
		shuffle(begin(init), end(init), rng);
		return init;
	}());
	bool notSorted = true;
	while (notSorted) {
		vector<int> curr(result.back());
		const auto size = curr.size();
		notSorted = false;
		for (auto i = 0U; i < size - 1; ++i) {
			if (curr[i] > curr[i + 1]) {
				notSorted = true;
				auto temp = curr[i + 1];
				curr[i + 1] = curr[i];
				curr[i] = temp;
			}
		}
		if (notSorted) {
			result.push_back(curr);
		}
	}
	return result;
}

void draw_hello_world(surface& rs) {
	rs.show_text("Hello world", { 100.0, 100.0 }, brush(solid_color_brush_factory(rgba_color::white())));
}

// For testing purposes only.
void draw_test_compositing_operators(surface& rs, double /*elapsedTimeInMilliseconds*/, compositing_operator secondRectCompOp, compositing_operator firstRectCompOp, bool strokePaths, const rgba_color& backgroundColor, const rgba_color& firstColor, const rgba_color& secondColor, bool clipToRects, bool clipToTriangle) {
	// Parameter validation.
	if (clipToRects && clipToTriangle) {
		throw invalid_argument("clipToRects and clipToTriangle cannot both be set to true.");
	}
	rs.save();

	auto backgroundBrush = brush(solid_color_brush_factory(backgroundColor));
	auto firstBrush = brush(solid_color_brush_factory(firstColor));
	auto secondBrush = brush(solid_color_brush_factory(secondColor));

	auto pb = path_factory();

	pb.rectangle({ 10.0, 10.0, 120.0, 90.0 });
	auto firstRectPath = path(pb);

	pb.clear();
	pb.rectangle({ 50.0, 40.0, 120.0, 90.0 });
	auto secondRectPath = path(pb);

	pb.clear();
	//pb.append(firstRectPath);
	//pb.append(secondRectPath);
	auto bothRectsClipPath = path(pb);

	pb.clear();
	pb.move_to({ 85.0, 25.0 });
	pb.line_to({ 150.0, 115.0 });
	pb.line_to({ 30.0, 115.0 });
	pb.close_path();
	auto triangleClipPath = path(pb);

	rs.brush(backgroundBrush);
	rs.compositing_operator(compositing_operator::source);
	rs.paint();

	rs.brush(firstBrush);
	rs.compositing_operator(firstRectCompOp);
	rs.path(firstRectPath);
	rs.fill();

	rs.save(); // Preserve old clip.
	if (clipToRects) {
		rs.clip(bothRectsClipPath);
	}

	if (clipToTriangle) {
		rs.clip(triangleClipPath);
	}

	rs.path(secondRectPath);
	rs.compositing_operator(secondRectCompOp);
	rs.brush(secondBrush);
	rs.fill();

	rs.restore(); // Restore old clip

	if (strokePaths) {
		rs.compositing_operator(compositing_operator::source);
		rs.line_width(2.0);

		rs.path(firstRectPath);
		rs.brush(brush(solid_color_brush_factory(rgba_color::teal())));
		rs.stroke();

		rs.path(secondRectPath);
		rs.brush(brush(solid_color_brush_factory(rgba_color::red())));
		rs.stroke();

		if (clipToTriangle) {
			rs.path(triangleClipPath);
			rs.brush(brush(solid_color_brush_factory(rgba_color::yellow())));
			rs.stroke();
		}
	}

	rs.restore();
}

void draw_sort_visualization_immediate(surface& rs, double elapsedTimeInMilliseconds) {
	static double timer = 0.0;
	const double power = 3.0;
	const double lerpTime = 1250.0;
	const double phaseTime = lerpTime + 500.0;
	const double normalizedTime = min(fmod(timer, phaseTime) / lerpTime, 1.0);
	const double adjustment = (normalizedTime < 0.5) ? pow(normalizedTime * 2.0, power) / 2.0 :
		((1.0 - pow(1.0 - ((normalizedTime - 0.5) * 2.0), power)) * 0.5) + 0.5;
	const int elementCount = 12;
	const static auto vec = init_sort_steps(elementCount);
	const auto phaseCount = vec.size();
	const size_t x = min(static_cast<size_t>(timer / phaseTime), max(phaseCount - 1U, 0U));
	rs.paint(rgba_color::cornflower_blue()); // Paint background.

	rs.immediate().clear();
	rs.immediate().arc({ 100.0, 100.0 }, 50.0, 0.0, pi / 2.0);
	auto initLineWidth = rs.line_width();
	rs.line_width(8.0);
	rs.stroke_immediate(rgba_color::black());
	rs.immediate().clear();
	rs.immediate().arc_negative({ 300.0, 100.0 }, 50.0, 0.0, pi / 2.0);
	rs.stroke_immediate(rgba_color::brown());
	rs.immediate().clear();
	rs.line_width(initLineWidth);

	auto clextents = rs.clip_extents();
	const double radius = trunc(min(clextents.width() * 0.8 / elementCount, clextents.height() + 120.0) / 2.0);
	const double beginX = trunc(clextents.width() * 0.1);
	const double y = trunc(clextents.height() * 0.5);

	auto linearTest1 = linear_brush_factory({ 400.0, 400.0 }, { 400.0, 500.0 });
	linearTest1.add_color_stop(0.0, rgba_color::black());
	linearTest1.add_color_stop(0.3, rgba_color::yellow());
	linearTest1.add_color_stop(0.5, rgba_color::blue());
	linearTest1.add_color_stop(0.3, rgba_color::lime());
	linearTest1.add_color_stop(0.5, rgba_color::black());
	linearTest1.add_color_stop(0.7, rgba_color::purple());
	linearTest1.add_color_stop(0.5, rgba_color::red());
//	linearTest1.add_color_stop(1.0, rgba_color::black());
	linearTest1.add_color_stop(0.7, rgba_color::orange());
	linearTest1.add_color_stop(0.8, rgba_color::green());
	linearTest1.add_color_stop(0.8, rgba_color::yellow());
	linearTest1.add_color_stop(1.0, rgba_color::white());
	rs.immediate().rectangle({ 400.0, 400.0, 200.0, 200.0 });
	rs.fill_immediate(brush(linearTest1));

	rs.font_face("Segoe UI", font_slant::normal, font_weight::normal);
	rs.font_size(40.0);
	auto str = string("Phase ").append(to_string(x + 1));
	rs.show_text(str, { beginX, 50.0 }, rgba_color::white());

	for (size_t i = 0; i < elementCount; ++i) {
		rs.immediate().clear();
		const auto currVal = vec[x][i];
		if (x < phaseCount - 1) {
			const auto i2 = find(begin(vec[x + 1]), end(vec[x + 1]), currVal) - begin(vec[x + 1]);
			const auto x1r = radius * i * 2.0 + radius + beginX, x2r = radius * i2 * 2.0 + radius + beginX;
			const auto yr = y - ((i2 == static_cast<int>(i) ? 0.0 : (radius * 4.0 * (normalizedTime < 0.5 ? normalizedTime : 1.0 - normalizedTime)))
				* (i % 2 == 1 ? 1.0 : -1.0));
			const auto center = vector_2d{ trunc((x2r - x1r) * adjustment + x1r), trunc(yr) };
			rs.immediate().transform_matrix(matrix_2d::init_scale({ 1.0, 1.5 }) * matrix_2d::init_rotate(pi / 4.0) * matrix_2d::init_translate({ 0.0, 50.0 }));
			rs.immediate().origin(center);
			rs.immediate().arc_negative(center, radius - 3.0, pi / 2.0, -pi / 2.0);
		}
		else {
			const vector_2d center{ radius * i * 2.0 + radius + beginX, y };
			rs.immediate().transform_matrix(matrix_2d::init_scale({ 1.0, 1.5 }) * matrix_2d::init_rotate(pi / 4.0));
			rs.immediate().origin(center);
			rs.immediate().arc_negative(center, radius - 3.0, pi / 2.0, -pi / 2.0);
		}
		double greyColor = 1.0 - (currVal / (elementCount - 1.0));
		rs.fill_immediate({ greyColor, greyColor, greyColor, 1.0 });
	}

	rs.immediate().clear();
	rs.immediate().origin({ 250.0, 450.0 });
	rs.immediate().transform_matrix(matrix_2d::init_shear_x(0.5).scale({ 2.0, 2.5 }));
	rs.immediate().rectangle({ 200.0, 400.0, 100.0, 100.0 });
	rs.line_width(3.0);
	rs.stroke_immediate(rgba_color::red());
	//auto radialFactory = radial_brush_factory({ 250.0, 450.0 }, 0.0, { 250.0, 450.0 }, 80.0);
	//radialFactory.add_color_stop(0.0, rgba_color::black());
	//radialFactory.add_color_stop(0.25, rgba_color::red());
	//radialFactory.add_color_stop(0.5, rgba_color::green());
	//radialFactory.add_color_stop(0.75, rgba_color::blue());
	//radialFactory.add_color_stop(1.0, rgba_color::white());
	//auto radialBrush = rs.create_brush(radialFactory);
	//radialBrush.extend(extend::reflect);
	//rs.fill_immediate(radialBrush);
	auto meshFactory = mesh_brush_factory();
	meshFactory.begin_patch();
	meshFactory.move_to({ 0.0, 0.0 });
	meshFactory.curve_to({ 30.0, -30.0 }, { 60.0, 30.0 }, { 100.0, 0.0 });
	meshFactory.curve_to({ 60.0, 30.0 }, { 130.0, 60.0 }, { 100.0, 100.0 });
	meshFactory.curve_to({ 60.0, 70.0 }, { 30.0, 130.0 }, { 0.0, 100.0 });
	meshFactory.curve_to({ 30.0, 70.0 }, { -30.0, 30.0 }, { 0.0, 0.0 });
	meshFactory.corner_color(0, rgba_color::red());
	meshFactory.corner_color(1, rgba_color::lime());
	meshFactory.corner_color(2, rgba_color::blue());
	meshFactory.corner_color(3, rgba_color::yellow());
	meshFactory.end_patch();
	meshFactory.begin_patch();
	meshFactory.move_to({ 100.0, 100.0 });
	meshFactory.line_to({ 130.0, 130.0 });
	meshFactory.line_to({ 130.0, 70.0 });
	meshFactory.corner_color(0, rgba_color::red());
	meshFactory.corner_color(1, rgba_color::lime());
	meshFactory.corner_color(2, rgba_color::blue());
	meshFactory.end_patch();
	auto meshBrush = brush(meshFactory);
	meshBrush.matrix(matrix_2d::init_translate({ -200.0, -400.0 }));
	rs.fill_immediate(meshBrush);

	auto imgSfc = image_surface(format::argb32, 40, 40);
	imgSfc.immediate().move_to({ 0.0, 0.0 });
	imgSfc.immediate().line_to({ 40.0, 40.0 });
	imgSfc.immediate().line_to({ 0.0, 40.0 });
	imgSfc.immediate().close_path();
	imgSfc.paint(rgba_color::green());
	imgSfc.fill_immediate(rgba_color::yellow());

	auto sfcFactory = surface_brush_factory(imgSfc);
	auto sfcBrush = brush(sfcFactory);
	sfcBrush.extend(extend::repeat);
	rs.immediate().clear();
	rs.immediate().rectangle({ 500.0, 450.0, 100.0, 100.0 });
	rs.immediate().rectangle({ 525.0, 425.0, 50.0, 150.0 });
	rs.line_join(line_join::miter_or_bevel);
	rs.miter_limit(1.0);
	rs.line_width(10.0);
	rs.stroke_immediate(rgba_color::red());
	rs.fill_immediate(sfcBrush);

	auto linearFactory = linear_brush_factory({ 510.0, 460.0 }, { 530.0, 480.0 });
	linearFactory.add_color_stop(0.0, rgba_color::chartreuse());
	linearFactory.add_color_stop(1.0, rgba_color::salmon());
	auto linearBrush = brush(linearFactory);
	linearBrush.extend(extend::repeat);
	rs.immediate().clear();
	rs.immediate().move_to({ 650.0, 400.0 });
	rs.immediate().rel_line_to({ 0.0, 100.0 });
	rs.immediate().rel_line_to({ 10.0, -100.0 });
	rs.line_join(line_join::miter);
	rs.stroke_immediate(rgba_color::red());
	rs.fill_immediate(linearBrush);

	rs.immediate().clear();
	rs.immediate().move_to({ 430.0, 60.0 });
	rs.immediate().arc({ 500.0, 60.0 }, 30.0, pi, pi * 2.0);
	rs.immediate().line_to({ 570.0, 60.0 });
//	rs.immediate().new_sub_path();
	rs.immediate().arc({ 500.0, 130.0 }, 30.0, pi * 2.0, pi * 3.0 / 4.0);
	rs.immediate().new_sub_path();
	rs.dashes(dashes{ { 0.0, 10.0 } , 0.0 });
	rs.line_width(5.0);
	rs.line_cap(line_cap::round);
	rs.fill_immediate(rgba_color::blue());
	rs.stroke_immediate(rgba_color::orange());
	// Reset dashes to be a solid line.
	rs.reset_dashes();
	rs.line_cap(line_cap::butt);

	rs.immediate().clear();
	rs.immediate().curve_to({ 610.0, 400.0 }, { 660.0, 300.0 }, { 710.0, 400.0 });
	rs.immediate().close_path();
	rs.stroke_immediate(rgba_color::yellow_green());

	//auto radialFactory = radial_brush_factory({ 115.2, 102.4 }, 25.6, { 102.4, 102.4 }, 128.0);
	//radialFactory.add_color_stop(0.0, rgba_color::white());
	////radialFactory.add_color_stop(0.25, rgba_color::red());
	////radialFactory.add_color_stop(0.5, rgba_color::green());
	////radialFactory.add_color_stop(0.75, rgba_color::blue());
	//radialFactory.add_color_stop(1.0, rgba_color::black());
	//auto radialBrush = brush(radialFactory);
	//radialBrush.extend(extend::pad);
	//rs.immediate().clear();
	////rs.immediate().arc({ 128.0, 128.0 }, 76.8, 0.0, 2 * pi);
	//rs.immediate().rect({ 0.0, 0.0, 250.0, 250.0 });
	//rs.fill_immediate(radialBrush);

	timer = (timer > phaseTime * (phaseCount + 2)) ? 0.0 : timer + elapsedTimeInMilliseconds;
}

void draw_sort_visualization(surface& rs, double elapsedTimeInMilliseconds) {
	static double timer = 0.0;
	const double power = 3.0;
	const double lerpTime = 1250.0;
	const double phaseTime = lerpTime + 500.0;
	const double normalizedTime = min(fmod(timer, phaseTime) / lerpTime, 1.0);
	const double adjustment = (normalizedTime < 0.5) ? pow(normalizedTime * 2.0, power) / 2.0 :
		((1.0 - pow(1.0 - ((normalizedTime - 0.5) * 2.0), power)) * 0.5) + 0.5;
	const int elementCount = 12;
	const static auto vec = init_sort_steps(elementCount);
	const auto phaseCount = vec.size();
	assert(phaseCount > 0);
	const size_t x = min(static_cast<size_t>(timer / phaseTime), max(phaseCount - 1U, 0U));
	auto cornflowerBlueBrush = brush(solid_color_brush_factory(rgba_color::cornflower_blue()));
	rs.brush(cornflowerBlueBrush);
	rs.paint(); // Paint background.

	auto clextents = rs.clip_extents();
	const double radius = trunc(min(clextents.width() * 0.8 / elementCount, clextents.height() + 120.0) / 2.0);
	const double beginX = trunc(clextents.width() * 0.1);
	const double y = trunc(clextents.height() * 0.5);

	auto whiteBrush = brush(solid_color_brush_factory(rgba_color::white()));
	rs.brush(whiteBrush);
	rs.font_face("Segoe UI", font_slant::normal, font_weight::normal);
	rs.font_size(40.0);
	rs.show_text(string("Phase ").append(to_string(x + 1)).c_str(), { beginX, 50.0 });

	path_factory pf;

	for (size_t i = 0; i < elementCount; ++i) {
		pf.clear();
		const auto currVal = vec[x][i];
		if (x < phaseCount - 1) {
			const auto i2 = find(begin(vec[x + 1]), end(vec[x + 1]), currVal) - begin(vec[x + 1]);
			const auto x1r = radius * i * 2.0 + radius + beginX, x2r = radius * i2 * 2.0 + radius + beginX;
			const auto yr = y - ((i2 == static_cast<int>(i) ? 0.0 : (radius * 4.0 * (normalizedTime < 0.5 ? normalizedTime : 1.0 - normalizedTime)))
				* (i % 2 == 1 ? 1.0 : -1.0));
			const auto center = vector_2d{ trunc((x2r - x1r) * adjustment + x1r), trunc(yr) };
			pf.transform_matrix(matrix_2d::init_scale({ 1.0, 1.5 }) * matrix_2d::init_rotate(pi / 4.0) * matrix_2d::init_translate({ 0.0, 50.0 }));
			pf.origin(center);
			pf.arc_negative(center, radius - 3.0, pi / 2.0, -pi / 2.0);
		}
		else {
			const vector_2d center{ radius * i * 2.0 + radius + beginX, y };
			pf.transform_matrix(matrix_2d::init_scale({ 1.0, 1.5 }) * matrix_2d::init_rotate(pi / 4.0));
			pf.origin(center);
			pf.arc_negative(center, radius - 3.0, pi / 2.0, -pi / 2.0);
		}
		rs.path(path(pf));
		double greyColor = 1.0 - (currVal / (elementCount - 1.0));
		auto greyBrush = brush(solid_color_brush_factory({ greyColor, greyColor, greyColor, 1.0 }));
		rs.brush(greyBrush);
		rs.fill();
	}

	pf.clear();
	pf.origin({ 250.0, 450.0 });
	pf.transform_matrix(matrix_2d::init_shear_x(0.5).scale({ 2.0, 1.0 }));
	pf.rectangle({ 200.0, 400.0, 100.0, 100.0 });
	rs.path(path(pf));
	auto redBrush = brush(solid_color_brush_factory(rgba_color::red()));
	rs.brush(redBrush);
	rs.line_width(3.0);
	rs.stroke();
	auto radialFactory = radial_brush_factory({ 250.0, 450.0 }, 0.0, { 250.0, 450.0 }, 80.0);
	radialFactory.add_color_stop(0.0, rgba_color::black());
	radialFactory.add_color_stop(0.25, rgba_color::red());
	radialFactory.add_color_stop(0.5, rgba_color::green());
	radialFactory.add_color_stop(0.75, rgba_color::blue());
	radialFactory.add_color_stop(1.0, rgba_color::white());
	auto radialBrush = brush(radialFactory);
	radialBrush.extend(extend::reflect);
	rs.brush(radialBrush);
	rs.fill();

	auto linearFactory = linear_brush_factory({ 510.0, 460.0 }, { 530.0, 480.0 });
	linearFactory.add_color_stop(0.0, rgba_color::chartreuse());
	linearFactory.add_color_stop(1.0, rgba_color::salmon());
	auto linearBrush = brush(linearFactory);
	linearBrush.extend(extend::repeat);
	pf.clear();
	pf.rectangle({ 500.0, 450.0, 100.0, 100.0 });
	pf.rectangle({ 525.0, 425.0, 50.0, 150.0 });
	rs.line_join(line_join::miter_or_bevel);
	rs.miter_limit(1.0);
	rs.line_width(10.0);
	rs.path(path(pf));
	rs.brush(redBrush);
	rs.stroke();
	rs.brush(linearBrush);
	rs.fill();

	pf.clear();
	pf.move_to({ 650.0, 400.0 });
	pf.rel_line_to({ 0.0, 100.0 });
	pf.rel_line_to({ 10.0, -100.0 });
	rs.line_join(line_join::miter);
	rs.path(path(pf));
	rs.brush(redBrush);
	rs.stroke();
	rs.brush(linearBrush);
	rs.fill();

	pf.clear();
	pf.move_to({ 430.0, 60.0 });
	pf.arc({ 500.0, 60.0 }, 30.0, pi, pi * 2.0);
	pf.line_to({ 570.0, 60.0 });
	pf.new_sub_path();
	pf.arc_negative({ 500.0, 130.0 }, 30.0, 0.0, pi * 3.0 / 4.0);
	pf.new_sub_path();
	rs.path(path(pf));
	rs.line_width(2.0);
	rs.brush(redBrush);
	rs.stroke();

	timer = (timer > phaseTime * (phaseCount + 2)) ? 0.0 : timer + elapsedTimeInMilliseconds;
}
