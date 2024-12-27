#include <gtkmm.h>
#include <cstdlib>
#include <sstream>

class BrightnessApp : public Gtk::Window {
public:
    BrightnessApp();

private:
    void on_slider_changed();
    void set_brightness(double brightness);

    Gtk::Box vbox{Gtk::ORIENTATION_VERTICAL};
    Gtk::Scale slider;
};

BrightnessApp::BrightnessApp() : slider(Gtk::ORIENTATION_HORIZONTAL) {
    set_title("Brightness");
    set_default_size(400, 100);
    slider.set_range(0.1, 1.0);  
    slider.set_value(1.0);
    slider.set_digits(2);
    slider.signal_value_changed().connect(sigc::mem_fun(*this, &BrightnessApp::on_slider_changed));

    vbox.pack_start(slider, Gtk::PACK_SHRINK);
    add(vbox);
    show_all_children();
}

void BrightnessApp::on_slider_changed() {
    double brightness = slider.get_value();
    set_brightness(brightness);
}

void BrightnessApp::set_brightness(double brightness) {
    std::ostringstream cmd;
    cmd << "xrandr --output VGA-0 --brightness " << brightness;
    std::system(cmd.str().c_str());
}

int main(int argc, char* argv[]) {
    auto app = Gtk::Application::create(argc, argv, "org.brightness.control");

    BrightnessApp app_window;
    return app->run(app_window);
}

