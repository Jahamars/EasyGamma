#include <gtkmm.h>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <iomanip>
#include <vector>
#include <string>

class BrightnessControlApp : public Gtk::Window {
public:
    BrightnessControlApp();

private:
    void on_slider_value_changed();
    void on_master_slider_value_changed();
    void set_gamma(const std::string& monitor, double red, double green, double blue);
    std::vector<std::string> get_connected_monitors();

    Gtk::Box vbox{Gtk::ORIENTATION_VERTICAL};
    Gtk::Scale red_slider, green_slider, blue_slider, master_slider;
    Gtk::Label label;
    Gtk::ComboBoxText monitor_combo;
};

BrightnessControlApp::BrightnessControlApp()
    : red_slider(Gtk::ORIENTATION_HORIZONTAL),
      green_slider(Gtk::ORIENTATION_HORIZONTAL),
      blue_slider(Gtk::ORIENTATION_HORIZONTAL),
      master_slider(Gtk::ORIENTATION_HORIZONTAL) {
    set_title("EasyGamma ");
    set_default_size(400, 300);

    // Initialize sliders for red, green, blue
    for (auto* slider : {&red_slider, &green_slider, &blue_slider, &master_slider}) {
        slider->set_range(0.1, 1.0);
        slider->set_value(1.0);
        slider->set_digits(2);
        vbox.pack_start(*slider, Gtk::PACK_SHRINK);
    }

    red_slider.signal_value_changed().connect(sigc::mem_fun(*this, &BrightnessControlApp::on_slider_value_changed));
    green_slider.signal_value_changed().connect(sigc::mem_fun(*this, &BrightnessControlApp::on_slider_value_changed));
    blue_slider.signal_value_changed().connect(sigc::mem_fun(*this, &BrightnessControlApp::on_slider_value_changed));
    master_slider.signal_value_changed().connect(sigc::mem_fun(*this, &BrightnessControlApp::on_master_slider_value_changed));

    // Get connected monitors and populate the ComboBox
    label.set_text("Select Monitor ");
    vbox.pack_start(label, Gtk::PACK_SHRINK);

    std::vector<std::string> monitors = get_connected_monitors();
    for (const auto& monitor : monitors) {
        monitor_combo.append(monitor);
    }
    if (!monitors.empty()) monitor_combo.set_active(0);
    vbox.pack_start(monitor_combo, Gtk::PACK_SHRINK);

    // Add elements to window
    add(vbox);
    show_all_children();
}

void BrightnessControlApp::on_slider_value_changed() {
    double red = red_slider.get_value();
    double green = green_slider.get_value();
    double blue = blue_slider.get_value();
    std::string monitor = monitor_combo.get_active_text();
    set_gamma(monitor, red, green, blue);
}

void BrightnessControlApp::on_master_slider_value_changed() {
    double master_value = master_slider.get_value();
    red_slider.set_value(master_value);
    green_slider.set_value(master_value);
    blue_slider.set_value(master_value);
}

void BrightnessControlApp::set_gamma(const std::string& monitor, double red, double green, double blue) {
    std::ostringstream gamma_stream;
    gamma_stream.imbue(std::locale("C"));
    gamma_stream << std::fixed << std::setprecision(2) << red << ":" << green << ":" << blue;

    std::string command = "xrandr --output " + monitor + " --gamma " + gamma_stream.str();
    std::cout << "Executing command: " << command << std::endl;
    std::system(command.c_str());
}

std::vector<std::string> BrightnessControlApp::get_connected_monitors() {
    std::vector<std::string> monitors;
    FILE* pipe = popen("xrandr | grep ' connected' | awk '{print $1}'", "r");
    if (!pipe) {
        std::cerr << "Error executing xrandr command." << std::endl;
        return monitors;
    }
    char buffer[128];
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        monitors.push_back(std::string(buffer).substr(0, std::string(buffer).find('\n')));
    }
    pclose(pipe);
    return monitors;
}

int main(int argc, char* argv[]) {
    auto app = Gtk::Application::create(argc, argv, "org.gtkmm.gamma_control");

    BrightnessControlApp brightness_control_app;
    return app->run(brightness_control_app);
}
