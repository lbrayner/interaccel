#include "settings.h"

Settings::Settings()
{
    sensitivity_ = 0.0;
    acceleration_ = 0.0;
    offset_ = 0.0;
    power_ = 0.0;
    pre_scale_x_ = 0.0;
    pre_scale_y_ = 0.0;
    post_scale_x_ = 0.0;
    post_scale_y_ = 0.0;
    angle_adjustment_ = 0.0;
    angle_snapping_ = 0.0;
    speed_cap_ = 0.0;
    fancy_output_ = false;
}

Settings::Settings(
    double sensitivity,
    double acceleration,
    double offset,
    double power,
    double pre_scale_x,
    double pre_scale_y,
    double post_scale_x,
    double post_scale_y,
    double angle_adjustment,
    double angle_snapping,
    double speed_cap,
    bool fancy_output
)
{
    sensitivity_ = sensitivity;
    acceleration_ = acceleration;
    offset_ = offset;
    power_ = power;
    pre_scale_x_ = pre_scale_x;
    pre_scale_y_ = pre_scale_y;
    post_scale_x_ = post_scale_x;
    post_scale_y_ = post_scale_y;
    angle_adjustment_ = angle_adjustment;
    angle_snapping_ = angle_snapping;
    speed_cap_ = speed_cap;
    fancy_output_ = fancy_output;
}

Settings::~Settings()
{
}
