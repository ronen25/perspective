// Attribition: The following resources were used in writing this module
// 1. http://paulbourke.net/miscellaneous/colourspace/
// 2. pos_grad_wsj - http://graphics.wsj.com/infectious-diseases-and-vaccines/
// 3. https://vega.github.io/vega/docs/schemes

#include <perspective/raw_types.h>
#include <perspective/base.h>
#include <vector>

namespace perspective
{

struct t_color
{
    typedef t_float64 value_type;

    t_color() {}

    t_color(t_int32 r_, t_int32 g, t_int32 b, t_int32 a = 0) {}

    t_color(const t_str& color)
    {
        PSP_VERBOSE_ASSERT(color.size() == 7 || color.size() == 9,
            "Unexpected color string encountered");
        int r, g, b, a;
        if (color.size() == 7)
        {
            sscanf(color.c_str(), "#%02x%02x%02x", &r, &g, &b);
        }

        if (color.size() == 9)
        {
            sscanf(color.c_str(), "#%02x%02x%02x%02x", &r, &g, &b, &a);
        }
    }

    t_color(t_float64 r_, t_float64 g_, t_float64 b_, t_float64 a_ = 1.0)
        : r(r_)
        , g(g_)
        , b(b_)
        , a(a_)
    {
    }

    t_color(const t_color& c, t_float64 a_)
        : r(c.r)
        , g(c.g)
        , b(c.b)
        , a(a_)
    {
    }

    t_color
    gradient(t_color c, t_float64 k) const
    {
        t_color rv;
        rv.r = r + (c.r - r) * k;
        rv.g = g + (c.g - g) * k;
        rv.b = b + (c.b - b) * k;
        rv.a = a + (c.a - a) * k;
        return rv;
    }

    void
    clear()
    {
        r = g = b = a = 0;
    }

    const t_color&
    opacity(t_float64 a_)
    {
        if (a_ < 0.0)
            a_ = 0.0;
        if (a_ > 1.0)
            a_ = 1.0;
        a = a_;
        return *this;
    }

    t_float64
    opacity() const
    {
        return a;
    }

    const t_color&
    premultiply()
    {
        r *= a;
        g *= a;
        b *= a;
        return *this;
    }

    const t_color&
    premultiply(t_float64 a_)
    {
        if (a <= 0.0 || a_ <= 0.0)
        {
            r = g = b = a = 0.0;
            return *this;
        }
        a_ /= a;
        r *= a_;
        g *= a_;
        b *= a_;
        a = a_;
        return *this;
    }

    const t_color&
    unmultiply()
    {
        if (a == 0)
        {
            r = g = b = 0;
            return *this;
        }
        t_float64 a_ = 1.0 / a;
        r *= a_;
        g *= a_;
        b *= a_;
        return *this;
    }

    t_float64 r;
    t_float64 g;
    t_float64 b;
    t_float64 a;
};

namespace color
{
    // clang-format off
namespace whites {
    t_color antique_white      {0.9804  , 0.9216  , 0.8431, 1.0000};
    t_color azure              {0.9412  , 1.0000  , 1.0000, 1.0000};
    t_color bisque             {1.0000  , 0.8941  , 0.7686, 1.0000};
    t_color blanched_almond    {1.0000  , 0.9216  , 0.8039, 1.0000};
    t_color cornsilk           {1.0000  , 0.9725  , 0.8627, 1.0000};
    t_color eggshell           {0.9900  , 0.9000  , 0.7900, 1.0000};
    t_color floral_white       {1.0000  , 0.9804  , 0.9412, 1.0000};
    t_color gainsboro          {0.8627  , 0.8627  , 0.8627, 1.0000};
    t_color ghost_white        {0.9725  , 0.9725  , 1.0000, 1.0000};
    t_color honeydew           {0.9412  , 1.0000  , 0.9412, 1.0000};
    t_color ivory              {1.0000  , 1.0000  , 0.9412, 1.0000};
    t_color lavender           {0.9020  , 0.9020  , 0.9804, 1.0000};
    t_color lavender_blush     {1.0000  , 0.9412  , 0.9608, 1.0000};
    t_color lemon_chiffon      {1.0000  , 0.9804  , 0.8039, 1.0000};
    t_color linen              {0.9804  , 0.9412  , 0.9020, 1.0000};
    t_color mint_cream         {0.9608  , 1.0000  , 0.9804, 1.0000};
    t_color misty_rose         {1.0000  , 0.8941  , 0.8824, 1.0000};
    t_color moccasin           {1.0000  , 0.8941  , 0.7098, 1.0000};
    t_color navajo_white       {1.0000  , 0.8706  , 0.6784, 1.0000};
    t_color old_lace           {0.9922  , 0.9608  , 0.9020, 1.0000};
    t_color papaya_whip        {1.0000  , 0.9373  , 0.8353, 1.0000};
    t_color peach_puff         {1.0000  , 0.8549  , 0.7255, 1.0000};
    t_color seashell           {1.0000  , 0.9608  , 0.9333, 1.0000};
    t_color snow               {1.0000  , 0.9804  , 0.9804, 1.0000};
    t_color thistle            {0.8471  , 0.7490  , 0.8471, 1.0000};
    t_color titanium_white     {0.9900  , 1.0000  , 0.9400, 1.0000};
    t_color wheat              {0.9608  , 0.8706  , 0.7020, 1.0000};
    t_color white              {1.0000  , 1.0000  , 1.0000, 1.0000};
    t_color white_smoke        {0.9608  , 0.9608  , 0.9608, 1.0000};
    t_color zinc_white         {0.9900  , 0.9700  , 1.0000, 1.0000};
}

namespace greys {
    t_color cold_grey          {0.5000  , 0.5400  , 0.5300, 1.0000};
    t_color dim_grey           {0.4118  , 0.4118  , 0.4118, 1.0000};
    t_color grey               {0.7529  , 0.7529  , 0.7529, 1.0000};
    t_color light_grey         {0.8275  , 0.8275  , 0.8275, 1.0000};
    t_color slate_grey         {0.4392  , 0.5020  , 0.5647, 1.0000};
    t_color slate_grey_dark    {0.1843  , 0.3098  , 0.3098, 1.0000};
    t_color slate_grey_light   {0.4667  , 0.5333  , 0.6000, 1.0000};
    t_color warm_grey          {0.5000  , 0.5000  , 0.4100, 1.0000};
}

namespace blacks {
    t_color black              {0.0000  , 0.0000  , 0.0000, 1.0000};
    t_color ivory_black        {0.1600  , 0.1400  , 0.1300, 1.0000};
    t_color lamp_black         {0.1800  , 0.2800  , 0.2300, 1.0000};
}

namespace reds {
    t_color alizarin_crimson   {0.8900  , 0.1500  , 0.2100, 1.0000};
    t_color brick              {0.6100  , 0.4000  , 0.1200, 1.0000};
    t_color cadmium_red_deep   {0.8900  , 0.0900  , 0.0500, 1.0000};
    t_color coral              {1.0000  , 0.4980  , 0.3137, 1.0000};
    t_color coral_light        {0.9412  , 0.5020  , 0.5020, 1.0000};
    t_color deep_pink          {1.0000  , 0.0784  , 0.5765, 1.0000};
    t_color english_red        {0.8300  , 0.2400  , 0.1000, 1.0000};
    t_color firebrick          {0.6980  , 0.1333  , 0.1333, 1.0000};
    t_color geranium_lake      {0.8900  , 0.0700  , 0.1900, 1.0000};
    t_color hot_pink           {1.0000  , 0.4118  , 0.7059, 1.0000};
    t_color indian_red         {0.6900  , 0.0900  , 0.1200, 1.0000};
    t_color light_salmon       {1.0000  , 0.6275  , 0.4784, 1.0000};
    t_color madder_lake_deep   {0.8900  , 0.1800  , 0.1900, 1.0000};
    t_color maroon             {0.6902  , 0.1882  , 0.3765, 1.0000};
    t_color pink               {1.0000  , 0.7529  , 0.7961, 1.0000};
    t_color pink_light         {1.0000  , 0.7137  , 0.7569, 1.0000};
    t_color raspberry          {0.5300  , 0.1500  , 0.3400, 1.0000};
    t_color red                {1.0000  , 0.0000  , 0.0000, 1.0000};
    t_color rose_madder        {0.8900  , 0.2100  , 0.2200, 1.0000};
    t_color salmon             {0.9804  , 0.5020  , 0.4471, 1.0000};
    t_color tomato             {1.0000  , 0.3882  , 0.2784, 1.0000};
    t_color venetian_red       {0.8300  , 0.1000  , 0.1200, 1.0000};
}

namespace browns {
    t_color beige              {0.6400  , 0.5800  , 0.5000, 1.0000};
    t_color brown              {0.5000  , 0.1647  , 0.1647, 1.0000};
    t_color brown_madder       {0.8600  , 0.1600  , 0.1600, 1.0000};
    t_color brown_ochre        {0.5300  , 0.2600  , 0.1200, 1.0000};
    t_color burlywood          {0.8706  , 0.7216  , 0.5294, 1.0000};
    t_color burnt_sienna       {0.5400  , 0.2100  , 0.0600, 1.0000};
    t_color burnt_umber        {0.5400  , 0.2000  , 0.1400, 1.0000};
    t_color chocolate          {0.8235  , 0.4118  , 0.1176, 1.0000};
    t_color deep_ochre         {0.4500  , 0.2400  , 0.1000, 1.0000};
    t_color flesh              {1.0000  , 0.4900  , 0.2500, 1.0000};
    t_color flesh_ochre        {1.0000  , 0.3400  , 0.1300, 1.0000};
    t_color gold_ochre         {0.7800  , 0.4700  , 0.1500, 1.0000};
    t_color greenish_umber     {1.0000  , 0.2400  , 0.0500, 1.0000};
    t_color khaki              {0.9412  , 0.9020  , 0.5490, 1.0000};
    t_color khaki_dark         {0.7412  , 0.7176  , 0.4196, 1.0000};
    t_color light_beige        {0.9608  , 0.9608  , 0.8627, 1.0000};
    t_color peru               {0.8039  , 0.5216  , 0.2471, 1.0000};
    t_color rosy_brown         {0.7373  , 0.5608  , 0.5608, 1.0000};
    t_color raw_sienna         {0.7800  , 0.3800  , 0.0800, 1.0000};
    t_color raw_umber          {0.4500  , 0.2900  , 0.0700, 1.0000};
    t_color sepia              {0.3700  , 0.1500  , 0.0700, 1.0000};
    t_color sienna             {0.6275  , 0.3216  , 0.1765, 1.0000};
    t_color saddle_brown       {0.5451  , 0.2706  , 0.0745, 1.0000};
    t_color sandy_brown        {0.9569  , 0.6431  , 0.3765, 1.0000};
    t_color tan                {0.8235  , 0.7059  , 0.5490, 1.0000};
    t_color van_dyke_brown     {0.3700  , 0.1500  , 0.0200, 1.0000};
}

namespace oranges {
    t_color cadmium_orange     {1.0000  , 0.3800  , 0.0100, 1.0000};
    t_color cadmium_red_light  {1.0000  , 0.0100  , 0.0500, 1.0000};
    t_color carrot             {0.9300  , 0.5700  , 0.1300, 1.0000};
    t_color dark_orange        {1.0000  , 0.5490  , 0.0000, 1.0000};
    t_color mars_orange        {0.5900  , 0.2700  , 0.0800, 1.0000};
    t_color mars_yellow        {0.8900  , 0.4400  , 0.1000, 1.0000};
    t_color orange             {1.0000  , 0.5000  , 0.0000, 1.0000};
    t_color orange_red         {1.0000  , 0.2706  , 0.0000, 1.0000};
    t_color yellow_ochre       {0.8900  , 0.5100  , 0.0900, 1.0000};
}

namespace yellows {
    t_color aureoline_yellow   {1.0000  , 0.6600  , 0.1400, 1.0000};
    t_color banana             {0.8900  , 0.8100  , 0.3400, 1.0000};
    t_color cadmium_lemon      {1.0000  , 0.8900  , 0.0100, 1.0000};
    t_color cadmium_yellow     {1.0000  , 0.6000  , 0.0700, 1.0000};
    t_color gold               {1.0000  , 0.8431  , 0.0000, 1.0000};
    t_color goldenrod          {0.8549  , 0.6471  , 0.1255, 1.0000};
    t_color goldenrod_dark     {0.7216  , 0.5255  , 0.0431, 1.0000};
    t_color goldenrod_light    {0.9804  , 0.9804  , 0.8235, 1.0000};
    t_color goldenrod_pale     {0.9333  , 0.9098  , 0.6667, 1.0000};
    t_color light_goldenrod    {0.9333  , 0.8667  , 0.5098, 1.0000};
    t_color melon              {0.8900  , 0.6600  , 0.4100, 1.0000};
    t_color naplesyellowdeep   {1.0000  , 0.6600  , 0.0700, 1.0000};
    t_color yellow             {1.0000  , 1.0000  , 0.0000, 1.0000};
    t_color yellow_light       {1.0000  , 1.0000  , 0.8784, 1.0000};
}

namespace greens {
    t_color chartreuse         {0.4980  , 1.0000  , 0.0000, 1.0000};
    t_color chromeoxidegreen   {0.4000  , 0.5000  , 0.0800, 1.0000};
    t_color cinnabar_green     {0.3800  , 0.7000  , 0.1600, 1.0000};
    t_color cobalt_green       {0.2400  , 0.5700  , 0.2500, 1.0000};
    t_color emerald_green      {0.0000  , 0.7900  , 0.3400, 1.0000};
    t_color forest_green       {0.1333  , 0.5451  , 0.1333, 1.0000};
    t_color green              {0.0000  , 1.0000  , 0.0000, 1.0000};
    t_color green_dark         {0.0000  , 0.3922  , 0.0000, 1.0000};
    t_color green_pale         {0.5961  , 0.9843  , 0.5961, 1.0000};
    t_color green_yellow       {0.6784  , 1.0000  , 0.1843, 1.0000};
    t_color lawn_green         {0.4863  , 0.9882  , 0.0000, 1.0000};
    t_color lime_green         {0.1961  , 0.8039  , 0.1961, 1.0000};
    t_color mint               {0.7400  , 0.9900  , 0.7900, 1.0000};
    t_color olive              {0.2300  , 0.3700  , 0.1700, 1.0000};
    t_color olive_drab         {0.4196  , 0.5569  , 0.1373, 1.0000};
    t_color olive_green_dark   {0.3333  , 0.4196  , 0.1843, 1.0000};
    t_color permanent_green    {0.0400  , 0.7900  , 0.1700, 1.0000};
    t_color sap_green          {0.1900  , 0.5000  , 0.0800, 1.0000};
    t_color sea_green          {0.1804  , 0.5451  , 0.3412, 1.0000};
    t_color sea_green_dark     {0.5608  , 0.7373  , 0.5608, 1.0000};
    t_color sea_green_medium   {0.2353  , 0.7020  , 0.4431, 1.0000};
    t_color sea_green_light    {0.1255  , 0.6980  , 0.6667, 1.0000};
    t_color spring_green       {0.0000  , 1.0000  , 0.4980, 1.0000};
    t_color spring_greenmedium {0.0000  , 0.9804  , 0.6039, 1.0000};
    t_color terre_verte        {0.2200  , 0.3700  , 0.0600, 1.0000};
    t_color viridian_light     {0.4300  , 1.0000  , 0.4400, 1.0000};
    t_color yellow_green       {0.6039  , 0.8039  , 0.1961, 1.0000};
}

namespace cyans {
    t_color aquamarine         {0.4980  , 1.0000  , 0.8314, 1.0000};
    t_color aquamarinemedium   {0.4000  , 0.8039  , 0.6667, 1.0000};
    t_color cyan               {0.0000  , 1.0000  , 1.0000, 1.0000};
    t_color cyan_white         {0.8784  , 1.0000  , 1.0000, 1.0000};
    t_color turquoise          {0.2510  , 0.8784  , 0.8157, 1.0000};
    t_color turquoise_dark     {0.0000  , 0.8078  , 0.8196, 1.0000};
    t_color turquoise_medium   {0.2824  , 0.8196  , 0.8000, 1.0000};
    t_color turquoise_pale     {0.6863  , 0.9333  , 0.9333, 1.0000};
}

namespace blues {
    t_color alice_blue         {0.9412  , 0.9725  , 1.0000, 1.0000};
    t_color blue               {0.0000  , 0.0000  , 1.0000, 1.0000};
    t_color blue_light         {0.6784  , 0.8471  , 0.9020, 1.0000};
    t_color blue_medium        {0.0000  , 0.0000  , 0.8039, 1.0000};
    t_color cadet              {0.3725  , 0.6196  , 0.6275, 1.0000};
    t_color cobalt             {0.2400  , 0.3500  , 0.6700, 1.0000};
    t_color cornflower         {0.3922  , 0.5843  , 0.9294, 1.0000};
    t_color cerulean           {0.0200  , 0.7200  , 0.8000, 1.0000};
    t_color dodger_blue        {0.1176  , 0.5647  , 1.0000, 1.0000};
    t_color indigo             {0.0300  , 0.1800  , 0.3300, 1.0000};
    t_color manganese_blue     {0.0100  , 0.6600  , 0.6200, 1.0000};
    t_color midnight_blue      {0.0980  , 0.0980  , 0.4392, 1.0000};
    t_color navy               {0.0000  , 0.0000  , 0.5020, 1.0000};
    t_color peacock            {0.2000  , 0.6300  , 0.7900, 1.0000};
    t_color powder_blue        {0.6902  , 0.8784  , 0.9020, 1.0000};
    t_color royal_blue         {0.2549  , 0.4118  , 0.8824, 1.0000};
    t_color slate_blue         {0.4157  , 0.3529  , 0.8039, 1.0000};
    t_color slate_blue_dark    {0.2824  , 0.2392  , 0.5451, 1.0000};
    t_color slate_blue_light   {0.5176  , 0.4392  , 1.0000, 1.0000};
    t_color slate_blue_medium  {0.4824  , 0.4078  , 0.9333, 1.0000};
    t_color sky_blue           {0.5294  , 0.8078  , 0.9216, 1.0000};
    t_color sky_blue_deep      {0.0000  , 0.7490  , 1.0000, 1.0000};
    t_color sky_blue_light     {0.5294  , 0.8078  , 0.9804, 1.0000};
    t_color steel_blue         {0.2745  , 0.5098  , 0.7059, 1.0000};
    t_color steel_blue_light   {0.6902  , 0.7686  , 0.8706, 1.0000};
    t_color turquoise_blue     {0.0000  , 0.7800  , 0.5500, 1.0000};
    t_color ultramarine        {0.0700  , 0.0400  , 0.5600, 1.0000};
}

namespace magentas {
    t_color blue_violet        {0.5412  , 0.1686  , 0.8863, 1.0000};
    t_color cobalt_violetdeep  {0.5700  , 0.1300  , 0.6200, 1.0000};
    t_color magenta            {1.0000  , 0.0000  , 1.0000, 1.0000};
    t_color orchid             {0.8549  , 0.4392  , 0.8392, 1.0000};
    t_color orchid_dark        {0.6000  , 0.1961  , 0.8000, 1.0000};
    t_color orchid_medium      {0.7294  , 0.3333  , 0.8275, 1.0000};
    t_color permanent_violet   {0.8600  , 0.1500  , 0.2700, 1.0000};
    t_color plum               {0.8667  , 0.6275  , 0.8667, 1.0000};
    t_color purple             {0.6275  , 0.1255  , 0.9412, 1.0000};
    t_color purple_medium      {0.5765  , 0.4392  , 0.8588, 1.0000};
    t_color ultramarine_violet {0.3600  , 0.1400  , 0.4300, 1.0000};
    t_color violet             {0.5600  , 0.3700  , 0.6000, 1.0000};
    t_color violet_dark        {0.5804  , 0.0000  , 0.8275, 1.0000};
    t_color violet_red         {0.8157  , 0.1255  , 0.5647, 1.0000};
    t_color violet_redmedium   {0.7804  , 0.0824  , 0.5216, 1.0000};
    t_color violet_red_pale    {0.8588  , 0.4392  , 0.5765, 1.0000};
}

    // clang-format on

    class t_gradient
    {
    private:
        typedef std::vector<t_color> t_cvec;
        struct t_stop
        {
            t_color m_color;
            t_float64 m_value;
        };
        std::vector<t_stop> m_stops;

    public:
        t_gradient() { canonical(); }

        t_gradient(
            t_float64 min, t_float64 max, const std::vector<t_color>& colors)
        {
            if (colors.empty())
                return;
            add_stops(min, max, colors);
        }

        t_gradient(const std::vector<t_color>& colors)
            : t_gradient(0, 1, colors)
        {
        }

        static t_gradient
        diverging_gradient(t_float64 min, t_float64 center, t_float64 max,
            const std::vector<t_color>& colors)
        {
            t_gradient rv;
            if (colors.empty())
                return rv;
            t_uint64 half = colors.size() / 2;
            rv.add_stops(
                min, center, t_cvec{colors.begin(), colors.begin() + half});
            rv.add_stops(
                center, max, t_cvec{colors.begin() + half, colors.end()});
            return rv;
        }

        void
        add_stops(
            t_float64 min, t_float64 max, const std::vector<t_color>& colors)
        {
            t_uindex idx = 0;
            t_float64 step = (max - min) / colors.size();
            for (const auto& c : colors)
            {
                add_stop(min + step * idx, c);
            }
        }

        void
        add_stop(
            t_float64 value, t_float64 red, t_float64 green, t_float64 blue)
        {
            add_stop(value, {red, green, blue, 1.});
        }

        void
        add_stop(t_float64 value, t_color c)
        {
            for (int i = 0; i < m_stops.size(); i++)
            {
                if (value < m_stops[i].m_value)
                {
                    m_stops.insert(m_stops.begin() + i, t_stop{c, value});
                    return;
                }
            }
            m_stops.push_back(t_stop({c, value}));
        }

        void
        clear()
        {
            m_stops.clear();
        }

        void
        canonical()
        {
            m_stops.clear();
            m_stops.push_back(t_stop{{0, 0, 1, 1}, 0.0f});  // Blue.
            m_stops.push_back(t_stop{{0, 1, 1, 1}, 0.25f}); // Cyan.
            m_stops.push_back(t_stop{{0, 1, 0, 1}, 0.5f});  // Green.
            m_stops.push_back(t_stop{{1, 1, 0, 1}, 0.75f}); // Yellow.
            m_stops.push_back(t_stop{{1, 0, 0, 1}, 1.0f});  // Red.
        }

        t_color
        sample(const t_float64 value)
        {
            t_color rv;

            if (m_stops.size() == 0)
                return rv;

            for (size_t i = 0, loop_end = m_stops.size(); i < loop_end; i++)
            {
                const auto& curr = m_stops[i];
                if (value < curr.m_value)
                {
                    const auto& prev = m_stops[std::max<int>(0, i - 1)];
                    auto vdiff = (prev.m_value - curr.m_value);

                    const auto& pc = prev.m_color;
                    const auto& cc = curr.m_color;

                    auto fract
                        = (vdiff == 0) ? 0 : (value - curr.m_value) / vdiff;
                    rv.r = (pc.r - cc.r) * fract + cc.r;
                    rv.g = (pc.g - cc.g) * fract + cc.g;
                    rv.b = (pc.b - cc.b) * fract + cc.b;
                    return rv;
                }
            }
            rv.r = m_stops.back().m_color.r;
            rv.g = m_stops.back().m_color.g;
            rv.b = m_stops.back().m_color.b;
            rv.a = 1.;
            return rv;
        }
    };

    class t_palette
    {
    public:
        t_palette(const std::vector<t_color>& colors)
            : m_colors(colors)
        {
        }

        const t_color& operator[](t_uindex idx) const
        {
            return m_colors.at(idx);
        }

        t_uindex
        size() const
        {
            return m_colors.size();
        }

    private:
        std::vector<t_color> m_colors;
    };

    namespace palettes
    {
        namespace categorical
        {
            t_palette
            accent()
            {
                return t_palette({
                    {"#7fc97f"},
                    {"#beaed4"},
                    {"#fdc086"},
                    {"#ffff99"},
                    {"#386cb0"},
                    {"#f0027f"},
                    {"#bf5b17"},
                    {"#666666"},
                });
            }

            t_palette
            category_10()
            {
                return t_palette({
                    {"#1f77b4"},
                    {"#ff7f0e"},
                    {"#2ca02c"},
                    {"#d62728"},
                    {"#9467bd"},
                    {"#8c564b"},
                    {"#e377c2"},
                    {"#7f7f7f"},
                    {"#bcbd22"},
                    {"#17becf"},
                });
            }

            t_palette
            category_20()
            {
                return t_palette({
                    {"#1f77b4"},
                    {"#aec7e8"},
                    {"#ff7f0e"},
                    {"#ffbb78"},
                    {"#2ca02c"},
                    {"#98df8a"},
                    {"#d62728"},
                    {"#ff9896"},
                    {"#9467bd"},
                    {"#c5b0d5"},
                    {"#8c564b"},
                    {"#c49c94"},
                    {"#e377c2"},
                    {"#f7b6d2"},
                    {"#7f7f7f"},
                    {"#c7c7c7"},
                    {"#bcbd22"},
                    {"#dbdb8d"},
                    {"#17becf"},
                    {"#9edae5"},
                });
            }

            t_palette
            category_20b()
            {
                return t_palette({
                    {"#393b79"},
                    {"#5254a3"},
                    {"#6b6ecf"},
                    {"#9c9ede"},
                    {"#637939"},
                    {"#8ca252"},
                    {"#b5cf6b"},
                    {"#cedb9c"},
                    {"#8c6d31"},
                    {"#bd9e39"},
                    {"#e7ba52"},
                    {"#e7cb94"},
                    {"#843c39"},
                    {"#ad494a"},
                    {"#d6616b"},
                    {"#e7969c"},
                    {"#7b4173"},
                    {"#a55194"},
                    {"#ce6dbd"},
                    {"#de9ed6"},
                });
            }

            t_palette
            category_20c()
            {
                return t_palette({
                    {"#3182bd"},
                    {"#6baed6"},
                    {"#9ecae1"},
                    {"#c6dbef"},
                    {"#e6550d"},
                    {"#fd8d3c"},
                    {"#fdae6b"},
                    {"#fdd0a2"},
                    {"#31a354"},
                    {"#74c476"},
                    {"#a1d99b"},
                    {"#c7e9c0"},
                    {"#756bb1"},
                    {"#9e9ac8"},
                    {"#bcbddc"},
                    {"#dadaeb"},
                    {"#636363"},
                    {"#969696"},
                    {"#bdbdbd"},
                    {"#d9d9d9"},
                });
            }

            t_palette
            dark_2()
            {
                return t_palette({
                    {"#1b9e77"},
                    {"#d95f02"},
                    {"#7570b3"},
                    {"#e7298a"},
                    {"#66a61e"},
                    {"#e6ab02"},
                    {"#a6761d"},
                    {"#666666"},
                });
            }

            t_palette
            paired()
            {
                return t_palette({
                    {"#a6cee3"},
                    {"#1f78b4"},
                    {"#b2df8a"},
                    {"#33a02c"},
                    {"#fb9a99"},
                    {"#e31a1c"},
                    {"#fdbf6f"},
                    {"#ff7f00"},
                    {"#cab2d6"},
                    {"#6a3d9a"},
                    {"#ffff99"},
                    {"#b15928"},
                });
            }

            t_palette
            pastel_1()
            {
                return t_palette({
                    {"#fbb4ae"},
                    {"#b3cde3"},
                    {"#ccebc5"},
                    {"#decbe4"},
                    {"#fed9a6"},
                    {"#ffffcc"},
                    {"#e5d8bd"},
                    {"#fddaec"},
                    {"#f2f2f2"},
                });
            }
            t_palette
            pastel_2()
            {
                return t_palette({
                    {"#b3e2cd"},
                    {"#fdcdac"},
                    {"#cbd5e8"},
                    {"#f4cae4"},
                    {"#e6f5c9"},
                    {"#fff2ae"},
                    {"#f1e2cc"},
                    {"#cccccc"},
                });
            }

            t_palette
            set_1()
            {
                return t_palette({
                    {"#e41a1c"},
                    {"#377eb8"},
                    {"#4daf4a"},
                    {"#984ea3"},
                    {"#ff7f00"},
                    {"#ffff33"},
                    {"#a65628"},
                    {"#f781bf"},
                    {"#999999"},
                });
            }
            t_palette
            set_2()
            {
                return t_palette({
                    {"#66c2a5"},
                    {"#fc8d62"},
                    {"#8da0cb"},
                    {"#e78ac3"},
                    {"#a6d854"},
                    {"#ffd92f"},
                    {"#e5c494"},
                    {"#b3b3b3"},
                });
            }

            t_palette
            set_3()
            {
                return t_palette({
                    {"#8dd3c7"},
                    {"#ffffb3"},
                    {"#bebada"},
                    {"#fb8072"},
                    {"#80b1d3"},
                    {"#fdb462"},
                    {"#b3de69"},
                    {"#fccde5"},
                    {"#d9d9d9"},
                    {"#bc80bd"},
                    {"#ccebc5"},
                    {"#ffed6f"},
                });
            }

            t_palette
            tableau_10()
            {
                return t_palette({
                    {"#4c78a8"},
                    {"#f58518"},
                    {"#e45756"},
                    {"#72b7b2"},
                    {"#54a24b"},
                    {"#eeca3b"},
                    {"#b279a2"},
                    {"#ff9da6"},
                    {"#9d755d"},
                    {"#bab0ac"},
                });
            }

            t_palette
            tableau_20()
            {
                return t_palette({
                    {"#4c78a8"},
                    {"#9ecae9"},
                    {"#f58518"},
                    {"#ffbf79"},
                    {"#54a24b"},
                    {"#88d27a"},
                    {"#b79a20"},
                    {"#f2cf5b"},
                    {"#439894"},
                    {"#83bcb6"},
                    {"#e45756"},
                    {"#ff9d98"},
                    {"#79706e"},
                    {"#bab0ac"},
                    {"#d67195"},
                    {"#fcbfd2"},
                    {"#b279a2"},
                    {"#d6a5c9"},
                    {"#9e765f"},
                    {"#d8b5a5"},
                });
            }
        } // namespace categorical

        namespace sequential
        {
            // clang-format off
            t_palette
            blues_3()
            {
                return t_palette({
                    {"#deebf7"},
                    {"#9ecae1"},
                    {"#3182bd"}
                });
            }

            t_palette
            blues_4()
            {
                return t_palette({
                    {"#eff3ff"},
                    {"#bdd7e7"},
                    {"#6baed6"},
                    {"#2171b5"}
                });
            }

            t_palette
            blues_5()
            {
                return t_palette({
                    {"#eff3ff"},
                    {"#bdd7e7"},
                    {"#6baed6"},
                    {"#3182bd"},
                    {"#08519c"}
                });
            }

            t_palette
            blues_6()
            {
                return t_palette({
                    {"#eff3ff"},
                    {"#c6dbef"},
                    {"#9ecae1"},
                    {"#6baed6"},
                    {"#3182bd"},
                    {"#08519c"},
                });
            }

            t_palette
            blues_7()
            {
                return t_palette({
                    {"#eff3ff"},
                    {"#c6dbef"},
                    {"#9ecae1"},
                    {"#6baed6"},
                    {"#4292c6"},
                    {"#2171b5"},
                    {"#084594"}
                });
            }
            t_palette
            blues_8()
            {
                return t_palette({
                    {"#f7fbff"},
                    {"#deebf7"},
                    {"#c6dbef"},
                    {"#9ecae1"},
                    {"#6baed6"},
                    {"#4292c6"},
                    {"#2171b5"},
                    {"#084594"}
                });
            }

            t_palette
            blues_9()
            {
                return t_palette({
                    {"#f7fbff"},
                    {"#deebf7"},
                    {"#c6dbef"},
                    {"#9ecae1"},
                    {"#6baed6"},
                    {"#4292c6"},
                    {"#2171b5"},
                    {"#08519c"},
                    {"#08306b"},
                });
            }

            t_palette
            greens_3()
            {
                return t_palette({
                    {"#e5f5e0"},
                    {"#a1d99b"},
                    {"#31a354"},
                });
            }

            t_palette
            greens_4()
            {
                return t_palette({
                    {"#edf8e9"},
                    {"#bae4b3"},
                    {"#74c476"},
                    {"#238b45"},
                });
            }

            t_palette
            greens_5()
            {
                return t_palette({
                    {"#edf8e9"},
                    {"#bae4b3"},
                    {"#74c476"},
                    {"#31a354"},
                    {"#006d2c"},
                });
            }

            t_palette
            greens_6()
            {
                return t_palette({
                    {"#edf8e9"},
                    {"#c7e9c0"},
                    {"#a1d99b"},
                    {"#74c476"},
                    {"#31a354"},
                    {"#006d2c"},
                });
            }

            t_palette
            greens_7()
            {
                return t_palette({
                    {"#edf8e9"},
                    {"#c7e9c0"},
                    {"#a1d99b"},
                    {"#74c476"},
                    {"#41ab5d"},
                    {"#238b45"},
                    {"#005a32"},
                });
            }

            t_palette
            greens_8()
            {
                return t_palette({
                    {"#f7fcf5"},
                    {"#e5f5e0"},
                    {"#c7e9c0"},
                    {"#a1d99b"},
                    {"#74c476"},
                    {"#41ab5d"},
                    {"#238b45"},
                    {"#005a32"},
                });
            }

            t_palette
            greens_9()
            {
                return t_palette({
                    {"#f7fcf5"},
                    {"#e5f5e0"},
                    {"#c7e9c0"},
                    {"#a1d99b"},
                    {"#74c476"},
                    {"#41ab5d"},
                    {"#238b45"},
                    {"#006d2c"},
                    {"#00441b"},
                });
            }

            t_palette
            greys_3()
            {
                return t_palette({
                    {"#f0f0f0"},
                    {"#bdbdbd"},
                    {"#636363"},
                });
            }

            t_palette
            greys_4()
            {
                return t_palette({
                    {"#f7f7f7"},
                    {"#cccccc"},
                    {"#969696"},
                    {"#525252"},
                });
            }

            t_palette
            greys_5()
            {
                return t_palette({
                    {"#f7f7f7"},
                    {"#cccccc"},
                    {"#969696"},
                    {"#636363"},
                    {"#252525"},
                });
            }

            t_palette
            greys_6()
            {
                return t_palette({
                    {"#f7f7f7"},
                    {"#d9d9d9"},
                    {"#bdbdbd"},
                    {"#969696"},
                    {"#636363"},
                    {"#252525"},
                });
            }
            t_palette
            greys_7()
            {
                return t_palette({
                    {"#f7f7f7"},
                    {"#d9d9d9"},
                    {"#bdbdbd"},
                    {"#969696"},
                    {"#737373"},
                    {"#525252"},
                    {"#252525"},
                });
            }

            t_palette
            greys_8()
            {
                return t_palette({
                    {"#ffffff"},
                    {"#f0f0f0"},
                    {"#d9d9d9"},
                    {"#bdbdbd"},
                    {"#969696"},
                    {"#737373"},
                    {"#525252"},
                    {"#252525"},
                });
            }

            t_palette
            greys_9()
            {
                return t_palette({
                    {"#ffffff"},
                    {"#f0f0f0"},
                    {"#d9d9d9"},
                    {"#bdbdbd"},
                    {"#969696"},
                    {"#737373"},
                    {"#525252"},
                    {"#252525"},
                    {"#000000"},
                });
            }

            t_palette
            purples_3()
            {
                return t_palette({
                    {"#efedf5"},
                    {"#bcbddc"},
                    {"#756bb1"},
                });
            }

            t_palette
            purples_4()
            {
                return t_palette({
                    {"#f2f0f7"},
                    {"#cbc9e2"},
                    {"#9e9ac8"},
                    {"#6a51a3"},
                });
            }

            t_palette
            purples_5()
            {
                return t_palette({
                    {"#f2f0f7"},
                    {"#cbc9e2"},
                    {"#9e9ac8"},
                    {"#756bb1"},
                    {"#54278f"},
                });
            }

            t_palette
            purples_6()
            {
                return t_palette({
                    {"#f2f0f7"},
                    {"#dadaeb"},
                    {"#bcbddc"},
                    {"#9e9ac8"},
                    {"#756bb1"},
                    {"#54278f"},
                });
            }

            t_palette
            purples_7()
            {
                return t_palette({
                    {"#f2f0f7"},
                    {"#dadaeb"},
                    {"#bcbddc"},
                    {"#9e9ac8"},
                    {"#807dba"},
                    {"#6a51a3"},
                    {"#4a1486"},
                });
            }

            t_palette
            purples_8()
            {
                return t_palette({
                    {"#fcfbfd"},
                    {"#efedf5"},
                    {"#dadaeb"},
                    {"#bcbddc"},
                    {"#9e9ac8"},
                    {"#807dba"},
                    {"#6a51a3"},
                    {"#4a1486"},
                });
            }

            t_palette
            purples_9()
            {
                return t_palette({
                    {"#fcfbfd"},
                    {"#efedf5"},
                    {"#dadaeb"},
                    {"#bcbddc"},
                    {"#9e9ac8"},
                    {"#807dba"},
                    {"#6a51a3"},
                    {"#54278f"},
                    {"#3f007d"},
                });
            }

            t_palette
            reds_3()
            {
                return t_palette({
                    {"#fee0d2"},
                    {"#fc9272"},
                    {"#de2d26"},
                });
            }

            t_palette
            reds_4()
            {
                return t_palette({
                    {"#fee5d9"},
                    {"#fcae91"},
                    {"#fb6a4a"},
                    {"#cb181d"},
                });
            }

            t_palette
            reds_5()
            {
                return t_palette({
                    {"#fee5d9"},
                    {"#fcae91"},
                    {"#fb6a4a"},
                    {"#de2d26"},
                    {"#a50f15"},
                });
            }
            
            t_palette
            reds_6()
            {
                return t_palette({
                    {"#fee5d9"},
                    {"#fcbba1"},
                    {"#fc9272"},
                    {"#fb6a4a"},
                    {"#de2d26"},
                    {"#a50f15"},
                });
            }

            t_palette
            reds_7()
            {
                return t_palette({
                    {"#fee5d9"},
                    {"#fcbba1"},
                    {"#fc9272"},
                    {"#fb6a4a"},
                    {"#ef3b2c"},
                    {"#cb181d"},
                    {"#99000d"},
                });
            }
            
            t_palette
            reds_8()
            {
                return t_palette({
                    {"#fff5f0"},
                    {"#fee0d2"},
                    {"#fcbba1"},
                    {"#fc9272"},
                    {"#fb6a4a"},
                    {"#ef3b2c"},
                    {"#cb181d"},
                    {"#99000d"},
                });
            }

            t_palette
            reds_9()
            {
                return t_palette({
                    {"#fff5f0"},
                    {"#fee0d2"},
                    {"#fcbba1"},
                    {"#fc9272"},
                    {"#fb6a4a"},
                    {"#ef3b2c"},
                    {"#cb181d"},
                    {"#a50f15"},
                    {"#67000d"},
                });
            }
            
            t_palette
            oranges_3()
            {
                return t_palette({
                    {"#fee6ce"},
                    {"#fdae6b"},
                    {"#e6550d"},
                });
            }

            t_palette
            oranges_4()
            {
                return t_palette({
                    {"#feedde"},
                    {"#fdbe85"},
                    {"#fd8d3c"},
                    {"#d94701"},
                });
            }

            t_palette
            oranges_5()
            {
                return t_palette({
                    {"#feedde"},
                    {"#fdbe85"},
                    {"#fd8d3c"},
                    {"#e6550d"},
                    {"#a63603"},
                });
            }

            t_palette
            oranges_6()
            {
                return t_palette({
                    {"#feedde"},
                    {"#fdd0a2"},
                    {"#fdae6b"},
                    {"#fd8d3c"},
                    {"#e6550d"},
                    {"#a63603"},
                });
            }
            
            t_palette
            oranges_7()
            {
                return t_palette({
                    {"#feedde"},
                    {"#fdd0a2"},
                    {"#fdae6b"},
                    {"#fd8d3c"},
                    {"#f16913"},
                    {"#d94801"},
                    {"#8c2d04"},
                });
            }

            t_palette
            oranges_8()
            {
                return t_palette({
                    {"#fff5eb"},
                    {"#fee6ce"},
                    {"#fdd0a2"},
                    {"#fdae6b"},
                    {"#fd8d3c"},
                    {"#f16913"},
                    {"#d94801"},
                    {"#8c2d04"},
                });
            }
            
            t_palette
            oranges_9()
            {
                return t_palette({
                    {"#fff5eb"},
                    {"#fee6ce"},
                    {"#fdd0a2"},
                    {"#fdae6b"},
                    {"#fd8d3c"},
                    {"#f16913"},
                    {"#d94801"},
                    {"#a63603"},
                    {"#7f2704"},
                });
            }

            namespace multi_hue {
            t_palette
            blue_green_3()
            {
                return t_palette({
                    {"#e5f5f9"},
                    {"#99d8c9"},
                    {"#2ca25f"},
                });
            }
            
            t_palette
            blue_green_4()
            {
                return t_palette({
                    {"#edf8fb"},
                    {"#b2e2e2"},
                    {"#66c2a4"},
                    {"#238b45"},
                });
            }

            t_palette
            blue_green_5()
            {
                return t_palette({
                    {"#edf8fb"},
                    {"#b2e2e2"},
                    {"#66c2a4"},
                    {"#2ca25f"},
                    {"#006d2c"},
                });
            }

            t_palette
            blue_green_6()
            {
                return t_palette({
                    {"#edf8fb"},
                    {"#ccece6"},
                    {"#99d8c9"},
                    {"#66c2a4"},
                    {"#2ca25f"},
                    {"#006d2c"},
                });
            }

            t_palette
            blue_green_7()
            {
                return t_palette({
                    {"#edf8fb"},
                    {"#ccece6"},
                    {"#99d8c9"},
                    {"#66c2a4"},
                    {"#41ae76"},
                    {"#238b45"},
                    {"#005824"},
                });
            }
            
            t_palette
            blue_green_8()
            {
                return t_palette({
                    {"#f7fcfd"},
                    {"#e5f5f9"},
                    {"#ccece6"},
                    {"#99d8c9"},
                    {"#66c2a4"},
                    {"#41ae76"},
                    {"#238b45"},
                    {"#005824"},
                });
            }

            t_palette
            blue_green_9()
            {
                return t_palette({
                    {"#f7fcfd"},
                    {"#e5f5f9"},
                    {"#ccece6"},
                    {"#99d8c9"},
                    {"#66c2a4"},
                    {"#41ae76"},
                    {"#238b45"},
                    {"#006d2c"},
                    {"#00441b"},
                });
            }
            
            t_palette
            blue_purple_3()
            {
                return t_palette({
                    {"#e0ecf4"},
                    {"#9ebcda"},
                    {"#8856a7"},
                });
            }

            t_palette
            blue_purple_4()
            {
                return t_palette({
                    {"#edf8fb"},
                    {"#b3cde3"},
                    {"#8c96c6"},
                    {"#88419d"},
                });
            }
            
            t_palette
            blue_purple_5()
            {
                return t_palette({
                    {"#edf8fb"},
                    {"#b3cde3"},
                    {"#8c96c6"},
                    {"#8856a7"},
                    {"#810f7c"},
                });
            }

            t_palette
            blue_purple_6()
            {
                return t_palette({
                    {"#edf8fb"},
                    {"#bfd3e6"},
                    {"#9ebcda"},
                    {"#8c96c6"},
                    {"#8856a7"},
                    {"#810f7c"},
                });
            }

            t_palette
            blue_purple_7()
            {
                return t_palette({
                    {"#edf8fb"},
                    {"#bfd3e6"},
                    {"#9ebcda"},
                    {"#8c96c6"},
                    {"#8c6bb1"},
                    {"#88419d"},
                    {"#6e016b"},
                });
            }

            t_palette
            blue_purple_8()
            {
                return t_palette({
                    {"#f7fcfd"},
                    {"#e0ecf4"},
                    {"#bfd3e6"},
                    {"#9ebcda"},
                    {"#8c96c6"},
                    {"#8c6bb1"},
                    {"#88419d"},
                    {"#6e016b"},
                });
            }
            
            t_palette
            blue_purple_9()
            {
                return t_palette({
                    {"#f7fcfd"},
                    {"#e0ecf4"},
                    {"#bfd3e6"},
                    {"#9ebcda"},
                    {"#8c96c6"},
                    {"#8c6bb1"},
                    {"#88419d"},
                    {"#810f7c"},
                    {"#4d004b"},
                });
            }

            t_palette
            green_blue_3()
            {
                return t_palette({
                    {"#e0f3db"},
                    {"#a8ddb5"},
                    {"#43a2ca"},
                });
            }
            
            t_palette
            green_blue_4()
            {
                return t_palette({
                    {"#f0f9e8"},
                    {"#bae4bc"},
                    {"#7bccc4"},
                    {"#2b8cbe"},
                });
            }

            t_palette
            green_blue_5()
            {
                return t_palette({
                    {"#f0f9e8"},
                    {"#bae4bc"},
                    {"#7bccc4"},
                    {"#43a2ca"},
                    {"#0868ac"},
                });
            }
            
            t_palette
            green_blue_6()
            {
                return t_palette({
                    {"#f0f9e8"},
                    {"#ccebc5"},
                    {"#a8ddb5"},
                    {"#7bccc4"},
                    {"#43a2ca"},
                    {"#0868ac"},
                });
            }

            t_palette
            green_blue_7()
            {
                return t_palette({
                    {"#f0f9e8"},
                    {"#ccebc5"},
                    {"#a8ddb5"},
                    {"#7bccc4"},
                    {"#4eb3d3"},
                    {"#2b8cbe"},
                    {"#08589e"},
                });
            }

            t_palette
            green_blue_8()
            {
                return t_palette({
                    {"#f7fcf0"},
                    {"#e0f3db"},
                    {"#ccebc5"},
                    {"#a8ddb5"},
                    {"#7bccc4"},
                    {"#4eb3d3"},
                    {"#2b8cbe"},
                    {"#08589e"},
                });
            }

            t_palette
            green_blue_9()
            {
                return t_palette({
                    {"#f7fcf0"},
                    {"#e0f3db"},
                    {"#ccebc5"},
                    {"#a8ddb5"},
                    {"#7bccc4"},
                    {"#4eb3d3"},
                    {"#2b8cbe"},
                    {"#0868ac"},
                    {"#084081"},
                });
            }
            
            t_palette
            orange_red_3()
            {
                return t_palette({
                    {"#fee8c8"},
                    {"#fdbb84"},
                    {"#e34a33"},
                });
            }

            t_palette
            orange_red_4()
            {
                return t_palette({
                    {"#fef0d9"},
                    {"#fdcc8a"},
                    {"#fc8d59"},
                    {"#d7301f"},
                });
            }

            t_palette
            orange_red_5()
            {
                return t_palette({
                    {"#fef0d9"},
                    {"#fdcc8a"},
                    {"#fc8d59"},
                    {"#e34a33"},
                    {"#b30000"},
                });
            }

            t_palette
            orange_red_6()
            {
                return t_palette({
                    {"#fef0d9"},
                    {"#fdd49e"},
                    {"#fdbb84"},
                    {"#fc8d59"},
                    {"#e34a33"},
                    {"#b30000"},
                });
            }

            t_palette
            orange_red_7()
            {
                return t_palette({
                    {"#fef0d9"},
                    {"#fdd49e"},
                    {"#fdbb84"},
                    {"#fc8d59"},
                    {"#ef6548"},
                    {"#d7301f"},
                    {"#990000"},
                });
            }

            t_palette
            orange_red_8()
            {
                return t_palette({
                    {"#fff7ec"},
                    {"#fee8c8"},
                    {"#fdd49e"},
                    {"#fdbb84"},
                    {"#fc8d59"},
                    {"#ef6548"},
                    {"#d7301f"},
                    {"#990000"},
                });
            }

            t_palette
            orange_red_9()
            {
                return t_palette({
                    {"#fff7ec"},
                    {"#fee8c8"},
                    {"#fdd49e"},
                    {"#fdbb84"},
                    {"#fc8d59"},
                    {"#ef6548"},
                    {"#d7301f"},
                    {"#b30000"},
                    {"#7f0000"},
                });
            }

            t_palette
            purple_blue_green_3()
            {
                return t_palette({
                    {"#ece2f0"},
                    {"#a6bddb"},
                    {"#1c9099"},
                });
            }

            t_palette
            purple_blue_green_4()
            {
                return t_palette({
                    {"#f6eff7"},
                    {"#bdc9e1"},
                    {"#67a9cf"},
                    {"#02818a"},
                });
            }

            t_palette
            purple_blue_green_5()
            {
                return t_palette({
                    {"#f6eff7"},
                    {"#bdc9e1"},
                    {"#67a9cf"},
                    {"#1c9099"},
                    {"#016c59"},
                });
            }

            t_palette
            purple_blue_green_6()
            {
                return t_palette({
                    {"#f6eff7"},
                    {"#d0d1e6"},
                    {"#a6bddb"},
                    {"#67a9cf"},
                    {"#1c9099"},
                    {"#016c59"},
                });
            }

            t_palette
            purple_blue_green_7()
            {
                return t_palette({
                    {"#f6eff7"},
                    {"#d0d1e6"},
                    {"#a6bddb"},
                    {"#67a9cf"},
                    {"#3690c0"},
                    {"#02818a"},
                    {"#016450"},
                });
            }

            t_palette
            purple_blue_green_8()
            {
                return t_palette({
                    {"#fff7fb"},
                    {"#ece2f0"},
                    {"#d0d1e6"},
                    {"#a6bddb"},
                    {"#67a9cf"},
                    {"#3690c0"},
                    {"#02818a"},
                    {"#016450"},
                });
            }

            t_palette
            purple_blue_green_9()
            {
                return t_palette({
                    {"#fff7fb"},
                    {"#ece2f0"},
                    {"#d0d1e6"},
                    {"#a6bddb"},
                    {"#67a9cf"},
                    {"#3690c0"},
                    {"#02818a"},
                    {"#016c59"},
                    {"#014636"},
                });
            }

            t_palette
            purple_blue_3()
            {
                return t_palette({
                    {"#ece7f2"},
                    {"#a6bddb"},
                    {"#2b8cbe"},
                });
            }

            t_palette
            purple_blue_4()
            {
                return t_palette({
                    {"#f1eef6"},
                    {"#bdc9e1"},
                    {"#74a9cf"},
                    {"#0570b0"},
                });
            }

            t_palette
            purple_blue_5()
            {
                return t_palette({
                    {"#f1eef6"},
                    {"#bdc9e1"},
                    {"#74a9cf"},
                    {"#2b8cbe"},
                    {"#045a8d"},
                });
            }

            t_palette
            purple_blue_6()
            {
                return t_palette({
                    {"#f1eef6"},
                    {"#d0d1e6"},
                    {"#a6bddb"},
                    {"#74a9cf"},
                    {"#2b8cbe"},
                    {"#045a8d"}, 
                });
            }

            t_palette
            purple_blue_7()
            {
                return t_palette({
                    {"#f1eef6"},
                    {"#d0d1e6"},
                    {"#a6bddb"},
                    {"#74a9cf"},
                    {"#3690c0"},
                    {"#0570b0"},
                    {"#034e7b"},
                });
            }

            t_palette
            purple_blue_8()
            {
                return t_palette({
                    {"#fff7fb"},
                    {"#ece7f2"},
                    {"#d0d1e6"},
                    {"#a6bddb"},
                    {"#74a9cf"},
                    {"#3690c0"},
                    {"#0570b0"},
                    {"#034e7b"},
                });
            }

            t_palette
            purple_blue_9()
            {
                return t_palette({
                    {"#fff7fb"},
                    {"#ece7f2"},
                    {"#d0d1e6"},
                    {"#a6bddb"},
                    {"#74a9cf"},
                    {"#3690c0"},
                    {"#0570b0"},
                    {"#045a8d"},
                    {"#023858"},
                });
            }

            t_palette
            purple_red_3()
            {
                return t_palette({
                    {"#e7e1ef"},
                    {"#c994c7"},
                    {"#dd1c77"},
                });
            }

            t_palette
            purple_red_4()
            {
                return t_palette({
                    {"#f1eef6"},
                    {"#d7b5d8"},
                    {"#df65b0"},
                    {"#ce1256"},
                });
            }

            t_palette
            purple_red_5()
            {
                return t_palette({
                    {"#f1eef6"},
                    {"#d7b5d8"},
                    {"#df65b0"},
                    {"#dd1c77"},
                    {"#980043"},
                });
            }

            t_palette
            purple_red_6()
            {
                return t_palette({
                    {"#f1eef6"},
                    {"#d4b9da"},
                    {"#c994c7"},
                    {"#df65b0"},
                    {"#dd1c77"},
                    {"#980043"},
                });
            }

            t_palette
            purple_red_7()
            {
                return t_palette({
                    {"#f1eef6"},
                    {"#d4b9da"},
                    {"#c994c7"},
                    {"#df65b0"},
                    {"#e7298a"},
                    {"#ce1256"},
                    {"#91003f"},
                });
            }

            t_palette
            purple_red_8()
            {
                return t_palette({
                    {"#f7f4f9"},
                    {"#e7e1ef"},
                    {"#d4b9da"},
                    {"#c994c7"},
                    {"#df65b0"},
                    {"#e7298a"},
                    {"#ce1256"},
                    {"#91003f"},
                });
            }

            t_palette
            purple_red_9()
            {
                return t_palette({
                    {"#f7f4f9"},
                    {"#e7e1ef"},
                    {"#d4b9da"},
                    {"#c994c7"},
                    {"#df65b0"},
                    {"#e7298a"},
                    {"#ce1256"},
                    {"#980043"},
                    {"#67001f"},
                    
                });
            }
            
            t_palette
            red_purple_3()
            {
                return t_palette({
                    {"#fde0dd"},
                    {"#fa9fb5"},
                    {"#c51b8a"},
                });
            }

            t_palette
            red_purple_4()
            {
                return t_palette({
                    {"#feebe2"},
                    {"#fbb4b9"},
                    {"#f768a1"},
                    {"#ae017e"},
                });
            }

            t_palette
            red_purple_5()
            {
                return t_palette({
                    {"#feebe2"},
                    {"#fbb4b9"},
                    {"#f768a1"},
                    {"#c51b8a"},
                    {"#7a0177"},
                });
            }

            t_palette
            red_purple_6()
            {
                return t_palette({
                    {"#feebe2"},
                    {"#fcc5c0"},
                    {"#fa9fb5"},
                    {"#f768a1"},
                    {"#c51b8a"},
                    {"#7a0177"},
                });
            }

            t_palette
            red_purple_7()
            {
                return t_palette({
                    {"#feebe2"},
                    {"#fcc5c0"},
                    {"#fa9fb5"},
                    {"#f768a1"},
                    {"#dd3497"},
                    {"#ae017e"},
                    {"#7a0177"},
                });
            }

            t_palette
            red_purple_8()
            {
                return t_palette({
                    {"#fff7f3"},
                    {"#fde0dd"},
                    {"#fcc5c0"},
                    {"#fa9fb5"},
                    {"#f768a1"},
                    {"#dd3497"},
                    {"#ae017e"},
                    {"#7a0177"},
                });
            }

            t_palette
            red_purple_9()
            {
                return t_palette({
                    {"#fff7f3"},
                    {"#fde0dd"},
                    {"#fcc5c0"},
                    {"#fa9fb5"},
                    {"#f768a1"},
                    {"#dd3497"},
                    {"#ae017e"},
                    {"#7a0177"},
                    {"#49006a"},
                });
            }

            t_palette
            yellow_green_blue_3()
            {
                return t_palette({
                    {"#edf8b1"},
                    {"#7fcdbb"},
                    {"#2c7fb8"},
                });
            }

            t_palette
            yellow_green_blue_4()
            {
                return t_palette({
                    {"#ffffcc"},
                    {"#a1dab4"},
                    {"#41b6c4"},
                    {"#225ea8"},
                });
            }

            t_palette
            yellow_green_blue_5()
            {
                return t_palette({
                    {"#ffffcc"},
                    {"#a1dab4"},
                    {"#41b6c4"},
                    {"#2c7fb8"},
                    {"#253494"},
                });
            }

            t_palette
            yellow_green_blue_6()
            {
                return t_palette({
                    {"#ffffcc"},
                    {"#c7e9b4"},
                    {"#7fcdbb"},
                    {"#41b6c4"},
                    {"#2c7fb8"},
                    {"#253494"},
                });
            }

            t_palette
            yellow_green_blue_7()
            {
                return t_palette({
                    {"#ffffcc"},
                    {"#c7e9b4"},
                    {"#7fcdbb"},
                    {"#41b6c4"},
                    {"#1d91c0"},
                    {"#225ea8"},
                    {"#0c2c84"},
                });
            }

            t_palette
            yellow_green_blue_8()
            {
                return t_palette({
                    {"#ffffd9"},
                    {"#edf8b1"},
                    {"#c7e9b4"},
                    {"#7fcdbb"},
                    {"#41b6c4"},
                    {"#1d91c0"},
                    {"#225ea8"},
                    {"#0c2c84"},
                });
            }

            t_palette
            yellow_green_blue_9()
            {
                return t_palette({
                    {"#ffffd9"},
                    {"#edf8b1"},
                    {"#c7e9b4"},
                    {"#7fcdbb"},
                    {"#41b6c4"},
                    {"#1d91c0"},
                    {"#225ea8"},
                    {"#253494"},
                    {"#081d58"},
                });
            }

            t_palette
            yellow_green_3()
            {
                return t_palette({
                    {"#f7fcb9"},
                    {"#addd8e"},
                    {"#31a354"},
                });
            }

            t_palette
            yellow_green_4()
            {
                return t_palette({
                    {"#ffffcc"},
                    {"#c2e699"},
                    {"#78c679"},
                    {"#238443"},
                });
            }

            t_palette
            yellow_green_5()
            {
                return t_palette({
                    {"#ffffcc"},
                    {"#c2e699"},
                    {"#78c679"},
                    {"#31a354"},
                    {"#006837"},
                });
            }

            t_palette
            yellow_green_6()
            {
                return t_palette({
                    {"#ffffcc"},
                    {"#d9f0a3"},
                    {"#addd8e"},
                    {"#78c679"},
                    {"#31a354"},
                    {"#006837"},
                });
            }

            t_palette
            yellow_green_7()
            {
                return t_palette({
                    {"#ffffcc"},
                    {"#d9f0a3"},
                    {"#addd8e"},
                    {"#78c679"},
                    {"#41ab5d"},
                    {"#238443"},
                    {"#005a32"},
                });
            }

            t_palette
            yellow_green_8()
            {
                return t_palette({
                    {"#ffffe5"},
                    {"#f7fcb9"},
                    {"#d9f0a3"},
                    {"#addd8e"},
                    {"#78c679"},
                    {"#41ab5d"},
                    {"#238443"},
                    {"#005a32"},
                });
            }

            t_palette
            yellow_green_9()
            {
                return t_palette({
                    {"#ffffe5"},
                    {"#f7fcb9"},
                    {"#d9f0a3"},
                    {"#addd8e"},
                    {"#78c679"},
                    {"#41ab5d"},
                    {"#238443"},
                    {"#006837"},
                    {"#004529"},
                });
            }

            t_palette
            yellow_orange_brown_3()
            {
                return t_palette({
                    {"#fff7bc"},
                    {"#fec44f"},
                    {"#d95f0e"},
                });
            }

            t_palette
            yellow_orange_brown_4()
            {
                return t_palette({
                    {"#ffffd4"},
                    {"#fed98e"},
                    {"#fe9929"},
                    {"#cc4c02"},
                });
            }

            t_palette
            yellow_orange_brown_5()
            {
                return t_palette({
                    {"#ffffd4"},
                    {"#fed98e"},
                    {"#fe9929"},
                    {"#d95f0e"},
                    {"#993404"},
                });
            }

            t_palette
            yellow_orange_brown_6()
            {
                return t_palette({
                    {"#ffffd4"},
                    {"#fee391"},
                    {"#fec44f"},
                    {"#fe9929"},
                    {"#d95f0e"},
                    {"#993404"},
                    
                });
            }

            t_palette
            yellow_orange_brown_7()
            {
                return t_palette({
                    {"#ffffd4"},
                    {"#fee391"},
                    {"#fec44f"},
                    {"#fe9929"},
                    {"#ec7014"},
                    {"#cc4c02"},
                    {"#8c2d04"},
                });
            }

            t_palette
            yellow_orange_brown_8()
            {
                return t_palette({
                    {"#ffffe5"},
                    {"#fff7bc"},
                    {"#fee391"},
                    {"#fec44f"},
                    {"#fe9929"},
                    {"#ec7014"},
                    {"#cc4c02"},
                    {"#8c2d04"},
                });
            }

            t_palette
            yellow_orange_brown_9()
            {
                return t_palette({
                    {"#ffffe5"},
                    {"#fff7bc"},
                    {"#fee391"},
                    {"#fec44f"},
                    {"#fe9929"},
                    {"#ec7014"},
                    {"#cc4c02"},
                    {"#993404"},
                    {"#662506"},
                });
            }

            t_palette
            yellow_orange_red_3()
            {
                return t_palette({
                    {"#ffeda0"},
                    {"#feb24c"},
                    {"#f03b20"},
                });
            }

            t_palette
            yellow_orange_red_4()
            {
                return t_palette({
                    {"#ffffb2"},
                    {"#fecc5c"},
                    {"#fd8d3c"},
                    {"#e31a1c"},
                });
            }

            t_palette
            yellow_orange_red_5()
            {
                return t_palette({
                    {"#ffffb2"},
                    {"#fecc5c"},
                    {"#fd8d3c"},
                    {"#f03b20"},
                    {"#bd0026"},
                });
            }

            t_palette
            yellow_orange_red_6()
            {
                return t_palette({
                    {"#ffffb2"},
                    {"#fed976"},
                    {"#feb24c"},
                    {"#fd8d3c"},
                    {"#f03b20"},
                    {"#bd0026"},
                });
            }

            t_palette
            yellow_orange_red_7()
            {
                return t_palette({
                    {"#ffffb2"},
                    {"#fed976"},
                    {"#feb24c"},
                    {"#fd8d3c"},
                    {"#fc4e2a"},
                    {"#e31a1c"},
                    {"#b10026"},
                });
            }

            t_palette
            yellow_orange_red_8()
            {
                return t_palette({
                    {"#ffffcc"},
                    {"#ffeda0"},
                    {"#fed976"},
                    {"#feb24c"},
                    {"#fd8d3c"},
                    {"#fc4e2a"},
                    {"#e31a1c"},
                    {"#b10026"},
                });
            }

            t_palette
            yellow_orange_red_9()
            {
                return t_palette({
                    {"#ffffcc"},
                    {"#ffeda0"},
                    {"#fed976"},
                    {"#feb24c"},
                    {"#fd8d3c"},
                    {"#fc4e2a"},
                    {"#e31a1c"},
                    {"#bd0026"},
                    {"#800026"},
                });
            }
            
            } // namespace multi_hue
            
        } // namespace sequential

        namespace diverging
        {
            t_palette
            blue_orange_3()
            {
                return t_palette({
                    {"#67a9cf"},
                    {"#f7f7f7"},
                    {"#f1a340"},
                });
            }

            t_palette
            blue_orange_4()
            {
                return t_palette({
                    {"#0571b0"},
                    {"#92c5de"},
                    {"#fdb863"},
                    {"#e66101"},
                });
            }

            t_palette
            blue_orange_5()
            {
                return t_palette({
                    {"#0571b0"},
                    {"#92c5de"},
                    {"#f7f7f7"},
                    {"#fdb863"},
                    {"#e66101"},
                });
            }

            t_palette
            blue_orange_6()
            {
                return t_palette({
                    {"#2166ac"},
                    {"#67a9cf"},
                    {"#d1e5f0"},
                    {"#fee0b6"},
                    {"#f1a340"},
                    {"#b35806"},
                });
            }

            t_palette
            blue_orange_7()
            {
                return t_palette({
                    {"#2166ac"},
                    {"#67a9cf"},
                    {"#d1e5f0"},
                    {"#f7f7f7"},
                    {"#fee0b6"},
                    {"#f1a340"},
                    {"#b35806"},
                });
            }

            t_palette
            blue_orange_8()
            {
                return t_palette({
                    {"#2166ac"},
                    {"#4393c3"},
                    {"#92c5de"},
                    {"#d1e5f0"},
                    {"#fee0b6"},
                    {"#fdb863"},
                    {"#e08214"},
                    {"#b35806"},
                });
            }

            t_palette
            blue_orange_9()
            {
                return t_palette({
                    {"#2166ac"},
                    {"#4393c3"},
                    {"#92c5de"},
                    {"#d1e5f0"},
                    {"#f7f7f7"},
                    {"#fee0b6"},
                    {"#fdb863"},
                    {"#e08214"},
                    {"#b35806"},
                });
            }

            t_palette
            blue_orange_10()
            {
                return t_palette({
                    {"#053061"},
                    {"#2166ac"},
                    {"#4393c3"},
                    {"#92c5de"},
                    {"#d1e5f0"},
                    {"#fee0b6"},
                    {"#fdb863"},
                    {"#e08214"},
                    {"#b35806"},
                    {"#7f3b08"},
                });
            }

            t_palette
            blue_orange_11()
            {
                return t_palette({
                    {"#053061"},
                    {"#2166ac"},
                    {"#4393c3"},
                    {"#92c5de"},
                    {"#d1e5f0"},
                    {"#f7f7f7"},
                    {"#fee0b6"},
                    {"#fdb863"},
                    {"#e08214"},
                    {"#b35806"},
                    {"#7f3b08"},
                });
            }

            t_palette
            brown_blue_green_3()
            {
                return t_palette({
                    {"#d8b365"},
                    {"#f5f5f5"},
                    {"#5ab4ac"},
                });
            }

            t_palette
            brown_blue_green_4()
            {
                return t_palette({
                    {"#a6611a"},
                    {"#dfc27d"},
                    {"#80cdc1"},
                    {"#018571"},
                });
            }

            t_palette
            brown_blue_green_5()
            {
                return t_palette({
                    {"#a6611a"},
                    {"#dfc27d"},
                    {"#f5f5f5"},
                    {"#80cdc1"},
                    {"#018571"},
                });
            }

            t_palette
            brown_blue_green_6()
            {
                return t_palette({
                    {"#8c510a"},
                    {"#d8b365"},
                    {"#f6e8c3"},
                    {"#c7eae5"},
                    {"#5ab4ac"},
                    {"#01665e"},
                });
            }

            t_palette
            brown_blue_green_7()
            {
                return t_palette({
                    {"#8c510a"},
                    {"#d8b365"},
                    {"#f6e8c3"},
                    {"#f5f5f5"},
                    {"#c7eae5"},
                    {"#5ab4ac"},
                    {"#01665e"},
                });
            }

            t_palette
            brown_blue_green_8()
            {
                return t_palette({
                    {"#8c510a"},
                    {"#bf812d"},
                    {"#dfc27d"},
                    {"#f6e8c3"},
                    {"#c7eae5"},
                    {"#80cdc1"},
                    {"#35978f"},
                    {"#01665e"},
                });
            }

            t_palette
            brown_blue_green_9()
            {
                return t_palette({
                    {"#8c510a"},
                    {"#bf812d"},
                    {"#dfc27d"},
                    {"#f6e8c3"},
                    {"#f5f5f5"},
                    {"#c7eae5"},
                    {"#80cdc1"},
                    {"#35978f"},
                    {"#01665e"},
                });
            }

            t_palette
            brown_blue_green_10()
            {
                return t_palette({
                    {"#543005"},
                    {"#8c510a"},
                    {"#bf812d"},
                    {"#dfc27d"},
                    {"#f6e8c3"},
                    {"#c7eae5"},
                    {"#80cdc1"},
                    {"#35978f"},
                    {"#01665e"},
                    {"#003c30"},
                });
            }

            t_palette
            brown_blue_green_11()
            {
                return t_palette({
                    {"#543005"},
                    {"#8c510a"},
                    {"#bf812d"},
                    {"#dfc27d"},
                    {"#f6e8c3"},
                    {"#f5f5f5"},
                    {"#c7eae5"},
                    {"#80cdc1"},
                    {"#35978f"},
                    {"#01665e"},
                    {"#003c30"},
                });
            }

            t_palette
            purple_green_3()
            {
                return t_palette({
                    {"#af8dc3"},
                    {"#f7f7f7"},
                    {"#7fbf7b"},
                });
            }

            t_palette
            purple_green_4()
            {
                return t_palette({
                    {"#7b3294"},
                    {"#c2a5cf"},
                    {"#a6dba0"},
                    {"#008837"},
                });
            }

            t_palette
            purple_green_5()
            {
                return t_palette({
                    {"#7b3294"},
                    {"#c2a5cf"},
                    {"#f7f7f7"},
                    {"#a6dba0"},
                    {"#008837"},
                });
            }

            t_palette
            purple_green_6()
            {
                return t_palette({
                    {"#762a83"},
                    {"#af8dc3"},
                    {"#e7d4e8"},
                    {"#d9f0d3"},
                    {"#7fbf7b"},
                    {"#1b7837"},
                });
            }

            t_palette
            purple_green_7()
            {
                return t_palette({
                    {"#762a83"},
                    {"#af8dc3"},
                    {"#e7d4e8"},
                    {"#f7f7f7"},
                    {"#d9f0d3"},
                    {"#7fbf7b"},
                    {"#1b7837"},
                });
            }

            t_palette
            purple_green_8()
            {
                return t_palette({
                    {"#762a83"},
                    {"#9970ab"},
                    {"#c2a5cf"},
                    {"#e7d4e8"},
                    {"#d9f0d3"},
                    {"#a6dba0"},
                    {"#5aae61"},
                    {"#1b7837"},
                });
            }

            t_palette
            purple_green_9()
            {
                return t_palette({
                    {"#762a83"},
                    {"#9970ab"},
                    {"#c2a5cf"},
                    {"#e7d4e8"},
                    {"#f7f7f7"},
                    {"#d9f0d3"},
                    {"#a6dba0"},
                    {"#5aae61"},
                    {"#1b7837"},
                });
            }

            t_palette
            purple_green_10()
            {
                return t_palette({
                    {"#40004b"},
                    {"#762a83"},
                    {"#9970ab"},
                    {"#c2a5cf"},
                    {"#e7d4e8"},
                    {"#d9f0d3"},
                    {"#a6dba0"},
                    {"#5aae61"},
                    {"#1b7837"},
                    {"#00441b"},
                });
            }

            t_palette
            purple_green_11()
            {
                return t_palette({
                    {"#40004b"},
                    {"#762a83"},
                    {"#9970ab"},
                    {"#c2a5cf"},
                    {"#e7d4e8"},
                    {"#f7f7f7"},
                    {"#d9f0d3"},
                    {"#a6dba0"},
                    {"#5aae61"},
                    {"#1b7837"},
                    {"#00441b"},
                });
            }

            t_palette
            pink_yellow_green_3()
            {
                return t_palette({
                    {"#e9a3c9"},
                    {"#f7f7f7"},
                    {"#a1d76a"},
                });
            }

            t_palette
            pink_yellow_green_4()
            {
                return t_palette({
                    {"#d01c8b"},
                    {"#f1b6da"},
                    {"#b8e186"},
                    {"#4dac26"},
                });
            }

            t_palette
            pink_yellow_green_5()
            {
                return t_palette({
                    {"#d01c8b"},
                    {"#f1b6da"},
                    {"#f7f7f7"},
                    {"#b8e186"},
                    {"#4dac26"},
                });
            }

            t_palette
            pink_yellow_green_6()
            {
                return t_palette({
                    {"#c51b7d"},
                    {"#e9a3c9"},
                    {"#fde0ef"},
                    {"#e6f5d0"},
                    {"#a1d76a"},
                    {"#4d9221"},
                });
            }

            t_palette
            pink_yellow_green_7()
            {
                return t_palette({
                    {"#c51b7d"},
                    {"#e9a3c9"},
                    {"#fde0ef"},
                    {"#f7f7f7"},
                    {"#e6f5d0"},
                    {"#a1d76a"},
                    {"#4d9221"},
                });
            }

            t_palette
            pink_yellow_green_8()
            {
                return t_palette({
                    {"#c51b7d"},
                    {"#de77ae"},
                    {"#f1b6da"},
                    {"#fde0ef"},
                    {"#e6f5d0"},
                    {"#b8e186"},
                    {"#7fbc41"},
                    {"#4d9221"},
                });
            }

            t_palette
            pink_yellow_green_9()
            {
                return t_palette({
                    {"#c51b7d"},
                    {"#de77ae"},
                    {"#f1b6da"},
                    {"#fde0ef"},
                    {"#f7f7f7"},
                    {"#e6f5d0"},
                    {"#b8e186"},
                    {"#7fbc41"},
                    {"#4d9221"},
                });
            }

            t_palette
            pink_yellow_green_10()
            {
                return t_palette({
                    {"#8e0152"},
                    {"#c51b7d"},
                    {"#de77ae"},
                    {"#f1b6da"},
                    {"#fde0ef"},
                    {"#e6f5d0"},
                    {"#b8e186"},
                    {"#7fbc41"},
                    {"#4d9221"},
                    {"#276419"},
                });
            }

            t_palette
            pink_yellow_green_11()
            {
                return t_palette({
                    {"#8e0152"},
                    {"#c51b7d"},
                    {"#de77ae"},
                    {"#f1b6da"},
                    {"#fde0ef"},
                    {"#f7f7f7"},
                    {"#e6f5d0"},
                    {"#b8e186"},
                    {"#7fbc41"},
                    {"#4d9221"},
                    {"#276419"},
                });
            }

            t_palette
            purple_orange_3()
            {
                return t_palette({
                    {"#998ec3"},
                    {"#f7f7f7"},
                    {"#f1a340"},
                });
            }

            t_palette
            purple_orange_4()
            {
                return t_palette({
                    {"#5e3c99"},
                    {"#b2abd2"},
                    {"#fdb863"},
                    {"#e66101"},
                });
            }

            t_palette
            purple_orange_5()
            {
                return t_palette({
                    {"#5e3c99"},
                    {"#b2abd2"},
                    {"#f7f7f7"},
                    {"#fdb863"},
                    {"#e66101"},
                });
            }

            t_palette
            purple_orange_6()
            {
                return t_palette({
                    {"#542788"},
                    {"#998ec3"},
                    {"#d8daeb"},
                    {"#fee0b6"},
                    {"#f1a340"},
                    {"#b35806"},
                });
            }

            t_palette
            purple_orange_7()
            {
                return t_palette({
                    {"#542788"},
                    {"#998ec3"},
                    {"#d8daeb"},
                    {"#f7f7f7"},
                    {"#fee0b6"},
                    {"#f1a340"},
                    {"#b35806"},
                });
            }

            t_palette
            purple_orange_8()
            {
                return t_palette({
                    {"#542788"},
                    {"#8073ac"},
                    {"#b2abd2"},
                    {"#d8daeb"},
                    {"#fee0b6"},
                    {"#fdb863"},
                    {"#e08214"},
                    {"#b35806"},
                });
            }

            t_palette
            purple_orange_9()
            {
                return t_palette({
                    {"#542788"},
                    {"#8073ac"},
                    {"#b2abd2"},
                    {"#d8daeb"},
                    {"#f7f7f7"},
                    {"#fee0b6"},
                    {"#fdb863"},
                    {"#e08214"},
                    {"#b35806"},
                });
            }

            t_palette
            purple_orange_10()
            {
                return t_palette({
                    {"#2d004b"},
                    {"#542788"},
                    {"#8073ac"},
                    {"#b2abd2"},
                    {"#d8daeb"},
                    {"#fee0b6"},
                    {"#fdb863"},
                    {"#e08214"},
                    {"#b35806"},
                    {"#7f3b08"},
                });
            }

            t_palette
            purple_orange_11()
            {
                return t_palette({
                    {"#2d004b"},
                    {"#542788"},
                    {"#8073ac"},
                    {"#b2abd2"},
                    {"#d8daeb"},
                    {"#f7f7f7"},
                    {"#fee0b6"},
                    {"#fdb863"},
                    {"#e08214"},
                    {"#b35806"},
                    {"#7f3b08"},
                });
            }

            t_palette
            red_blue_3()
            {
                return t_palette({
                    {"#ef8a62"},
                    {"#f7f7f7"},
                    {"#67a9cf"},
                });
            }

            t_palette
            red_blue_4()
            {
                return t_palette({
                    {"#ca0020"},
                    {"#f4a582"},
                    {"#92c5de"},
                    {"#0571b0"},
                });
            }

            t_palette
            red_blue_5()
            {
                return t_palette({
                    {"#ca0020"},
                    {"#f4a582"},
                    {"#f7f7f7"},
                    {"#92c5de"},
                    {"#0571b0"},
                });
            }

            t_palette
            red_blue_6()
            {
                return t_palette({
                    {"#b2182b"},
                    {"#ef8a62"},
                    {"#fddbc7"},
                    {"#d1e5f0"},
                    {"#67a9cf"},
                    {"#2166ac"},
                });
            }

            t_palette
            red_blue_7()
            {
                return t_palette({
                    {"#b2182b"},
                    {"#ef8a62"},
                    {"#fddbc7"},
                    {"#f7f7f7"},
                    {"#d1e5f0"},
                    {"#67a9cf"},
                    {"#2166ac"},
                });
            }

            t_palette
            red_blue_8()
            {
                return t_palette({
                    {"#b2182b"},
                    {"#d6604d"},
                    {"#f4a582"},
                    {"#fddbc7"},
                    {"#d1e5f0"},
                    {"#92c5de"},
                    {"#4393c3"},
                    {"#2166ac"},
                });
            }

            t_palette
            red_blue_9()
            {
                return t_palette({
                    {"#b2182b"},
                    {"#d6604d"},
                    {"#f4a582"},
                    {"#fddbc7"},
                    {"#f7f7f7"},
                    {"#d1e5f0"},
                    {"#92c5de"},
                    {"#4393c3"},
                    {"#2166ac"},
                });
            }

            t_palette
            red_blue_10()
            {
                return t_palette({
                    {"#67001f"},
                    {"#b2182b"},
                    {"#d6604d"},
                    {"#f4a582"},
                    {"#fddbc7"},
                    {"#d1e5f0"},
                    {"#92c5de"},
                    {"#4393c3"},
                    {"#2166ac"},
                    {"#053061"},
                });
            }

            t_palette
            red_blue_11()
            {
                return t_palette({
                    {"#67001f"},
                    {"#b2182b"},
                    {"#d6604d"},
                    {"#f4a582"},
                    {"#fddbc7"},
                    {"#f7f7f7"},
                    {"#d1e5f0"},
                    {"#92c5de"},
                    {"#4393c3"},
                    {"#2166ac"},
                    {"#053061"},
                });
            }

            t_palette
            red_grey_3()
            {
                return t_palette({
                    {"#ef8a62"},
                    {"#ffffff"},
                    {"#999999"},
                });
            }

            t_palette
            red_grey_4()
            {
                return t_palette({
                    {"#ca0020"},
                    {"#f4a582"},
                    {"#bababa"},
                    {"#404040"},
                });
            }

            t_palette
            red_grey_5()
            {
                return t_palette({
                    {"#ca0020"},
                    {"#f4a582"},
                    {"#ffffff"},
                    {"#bababa"},
                    {"#404040"},
                });
            }

            t_palette
            red_grey_6()
            {
                return t_palette({
                    {"#b2182b"},
                    {"#ef8a62"},
                    {"#fddbc7"},
                    {"#e0e0e0"},
                    {"#999999"},
                    {"#4d4d4d"},
                });
            }

            t_palette
            red_grey_7()
            {
                return t_palette({
                    {"#b2182b"},
                    {"#ef8a62"},
                    {"#fddbc7"},
                    {"#ffffff"},
                    {"#e0e0e0"},
                    {"#999999"},
                    {"#4d4d4d"},
                });
            }

            t_palette
            red_grey_8()
            {
                return t_palette({
                    {"#b2182b"},
                    {"#d6604d"},
                    {"#f4a582"},
                    {"#fddbc7"},
                    {"#e0e0e0"},
                    {"#bababa"},
                    {"#878787"},
                    {"#4d4d4d"},
                });
            }

            t_palette
            red_grey_9()
            {
                return t_palette({
                    {"#b2182b"},
                    {"#d6604d"},
                    {"#f4a582"},
                    {"#fddbc7"},
                    {"#ffffff"},
                    {"#e0e0e0"},
                    {"#bababa"},
                    {"#878787"},
                    {"#4d4d4d"},
                });
            }

            t_palette
            red_grey_10()
            {
                return t_palette({
                    {"#67001f"},
                    {"#b2182b"},
                    {"#d6604d"},
                    {"#f4a582"},
                    {"#fddbc7"},
                    {"#e0e0e0"},
                    {"#bababa"},
                    {"#878787"},
                    {"#4d4d4d"},
                    {"#1a1a1a"},
                });
            }

            t_palette
            red_grey_11()
            {
                return t_palette({
                    {"#67001f"},
                    {"#b2182b"},
                    {"#d6604d"},
                    {"#f4a582"},
                    {"#fddbc7"},
                    {"#ffffff"},
                    {"#e0e0e0"},
                    {"#bababa"},
                    {"#878787"},
                    {"#4d4d4d"},
                    {"#1a1a1a"},
                });
            }

            t_palette
            red_yellow_blue_3()
            {
                return t_palette({
                    {"#fc8d59"},
                    {"#ffffbf"},
                    {"#91bfdb"},
                });
            }

            t_palette
            red_yellow_blue_4()
            {
                return t_palette({
                    {"#d7191c"},
                    {"#fdae61"},
                    {"#abd9e9"},
                    {"#2c7bb6"},
                });
            }

            t_palette
            red_yellow_blue_5()
            {
                return t_palette({
                    {"#d7191c"},
                    {"#fdae61"},
                    {"#ffffbf"},
                    {"#abd9e9"},
                    {"#2c7bb6"},
                });
            }

            t_palette
            red_yellow_blue_6()
            {
                return t_palette({
                    {"#d73027"},
                    {"#fc8d59"},
                    {"#fee090"},
                    {"#e0f3f8"},
                    {"#91bfdb"},
                    {"#4575b4"},
                });
            }

            t_palette
            red_yellow_blue_7()
            {
                return t_palette({
                    {"#d73027"},
                    {"#fc8d59"},
                    {"#fee090"},
                    {"#ffffbf"},
                    {"#e0f3f8"},
                    {"#91bfdb"},
                    {"#4575b4"},
                });
            }

            t_palette
            red_yellow_blue_8()
            {
                return t_palette({
                    {"#d73027"},
                    {"#f46d43"},
                    {"#fdae61"},
                    {"#fee090"},
                    {"#e0f3f8"},
                    {"#abd9e9"},
                    {"#74add1"},
                    {"#4575b4"},
                });
            }

            t_palette
            red_yellow_blue_9()
            {
                return t_palette({
                    {"#d73027"},
                    {"#f46d43"},
                    {"#fdae61"},
                    {"#fee090"},
                    {"#ffffbf"},
                    {"#e0f3f8"},
                    {"#abd9e9"},
                    {"#74add1"},
                    {"#4575b4"},
                });
            }

            t_palette
            red_yellow_blue_10()
            {
                return t_palette({
                    {"#a50026"},
                    {"#d73027"},
                    {"#f46d43"},
                    {"#fdae61"},
                    {"#fee090"},
                    {"#e0f3f8"},
                    {"#abd9e9"},
                    {"#74add1"},
                    {"#4575b4"},
                    {"#313695"},
                });
            }

            t_palette
            red_yellow_blue_11()
            {
                return t_palette({
                    {"#a50026"},
                    {"#d73027"},
                    {"#f46d43"},
                    {"#fdae61"},
                    {"#fee090"},
                    {"#ffffbf"},
                    {"#e0f3f8"},
                    {"#abd9e9"},
                    {"#74add1"},
                    {"#4575b4"},
                    {"#313695"},
                });
            }

            t_palette
            red_yellow_green_3()
            {
                return t_palette({
                    {"#fc8d59"},
                    {"#ffffbf"},
                    {"#91cf60"},
                });
            }

            t_palette
            red_yellow_green_4()
            {
                return t_palette({
                    {"#d7191c"},
                    {"#fdae61"},
                    {"#a6d96a"},
                    {"#1a9641"},
                });
            }

            t_palette
            red_yellow_green_5()
            {
                return t_palette({
                    {"#d7191c"},
                    {"#fdae61"},
                    {"#ffffbf"},
                    {"#a6d96a"},
                    {"#1a9641"},
                });
            }

            t_palette
            red_yellow_green_6()
            {
                return t_palette({
                    {"#d73027"},
                    {"#fc8d59"},
                    {"#fee08b"},
                    {"#d9ef8b"},
                    {"#91cf60"},
                    {"#1a9850"},
                });
            }

            t_palette
            red_yellow_green_7()
            {
                return t_palette({
                    {"#d73027"},
                    {"#fc8d59"},
                    {"#fee08b"},
                    {"#ffffbf"},
                    {"#d9ef8b"},
                    {"#91cf60"},
                    {"#1a9850"},
                });
            }

            t_palette
            red_yellow_green_8()
            {
                return t_palette({
                    {"#d73027"},
                    {"#f46d43"},
                    {"#fdae61"},
                    {"#fee08b"},
                    {"#d9ef8b"},
                    {"#a6d96a"},
                    {"#66bd63"},
                    {"#1a9850"},
                });
            }

            t_palette
            red_yellow_green_9()
            {
                return t_palette({
                    {"#d73027"},
                    {"#f46d43"},
                    {"#fdae61"},
                    {"#fee08b"},
                    {"#ffffbf"},
                    {"#d9ef8b"},
                    {"#a6d96a"},
                    {"#66bd63"},
                    {"#1a9850"},
                });
            }

            t_palette
            red_yellow_green_10()
            {
                return t_palette({
                    {"#a50026"},
                    {"#d73027"},
                    {"#f46d43"},
                    {"#fdae61"},
                    {"#fee08b"},
                    {"#d9ef8b"},
                    {"#a6d96a"},
                    {"#66bd63"},
                    {"#1a9850"},
                    {"#006837"},
                });
            }

            t_palette
            red_yellow_green_11()
            {
                return t_palette({
                    {"#a50026"},
                    {"#d73027"},
                    {"#f46d43"},
                    {"#fdae61"},
                    {"#fee08b"},
                    {"#ffffbf"},
                    {"#d9ef8b"},
                    {"#a6d96a"},
                    {"#66bd63"},
                    {"#1a9850"},
                    {"#006837"},
                });
            }

            t_palette
            spectral_3()
            {
                return t_palette({
                    {"#fc8d59"},
                    {"#ffffbf"},
                    {"#99d594"},
                });
            }

            t_palette
            spectral_4()
            {
                return t_palette({
                    {"#d7191c"},
                    {"#fdae61"},
                    {"#abdda4"},
                    {"#2b83ba"},
                });
            }

            t_palette
            spectral_5()
            {
                return t_palette({
                    {"#d7191c"},
                    {"#fdae61"},
                    {"#ffffbf"},
                    {"#abdda4"},
                    {"#2b83ba"},
                });
            }

            t_palette
            spectral_6()
            {
                return t_palette({
                    {"#d53e4f"},
                    {"#fc8d59"},
                    {"#fee08b"},
                    {"#e6f598"},
                    {"#99d594"},
                    {"#3288bd"},
                });
            }

            t_palette
            spectral_7()
            {
                return t_palette({
                    {"#d53e4f"},
                    {"#fc8d59"},
                    {"#fee08b"},
                    {"#ffffbf"},
                    {"#e6f598"},
                    {"#99d594"},
                    {"#3288bd"},
                });
            }

            t_palette
            spectral_8()
            {
                return t_palette({
                    {"#d53e4f"},
                    {"#f46d43"},
                    {"#fdae61"},
                    {"#fee08b"},
                    {"#e6f598"},
                    {"#abdda4"},
                    {"#66c2a5"},
                    {"#3288bd"},
                });
            }

            t_palette
            spectral_9()
            {
                return t_palette({
                    {"#d53e4f"},
                    {"#f46d43"},
                    {"#fdae61"},
                    {"#fee08b"},
                    {"#ffffbf"},
                    {"#e6f598"},
                    {"#abdda4"},
                    {"#66c2a5"},
                    {"#3288bd"},
                });
            }

            t_palette
            spectral_10()
            {
                return t_palette({
                    {"#9e0142"},
                    {"#d53e4f"},
                    {"#f46d43"},
                    {"#fdae61"},
                    {"#fee08b"},
                    {"#e6f598"},
                    {"#abdda4"},
                    {"#66c2a5"},
                    {"#3288bd"},
                    {"#5e4fa2"},
                });
            }

            t_palette
            spectral_11()
            {
                return t_palette({
                    {"#9e0142"},
                    {"#d53e4f"},
                    {"#f46d43"},
                    {"#fdae61"},
                    {"#fee08b"},
                    {"#ffffbf"},
                    {"#e6f598"},
                    {"#abdda4"},
                    {"#66c2a5"},
                    {"#3288bd"},
                    {"#5e4fa2"},
                });
            }

            // clang-format on
        } // namespace diverging
    }     // namespace palettes

    namespace gradients
    {
        namespace sequential
        {
            t_gradient
            pos_grad_wsj()
            {
                t_gradient rv;
                rv.add_stop(0.00, {"#e7f0fa"});
                rv.add_stop(0.01, {"#c9e2f6"});
                rv.add_stop(0.02, {"#95cbee"});
                rv.add_stop(0.03, {"#0099dc"});
                rv.add_stop(0.09, {"#4ab04a"});
                rv.add_stop(0.10, {"#ffd73e"});
                rv.add_stop(0.15, {"#eec73a"});
                rv.add_stop(0.25, {"#e29421"});
                rv.add_stop(0.40, {"#e29421"});
                rv.add_stop(0.50, {"#f05336"});
                rv.add_stop(1.00, {"#ce472e"});
                return rv;
            }

            // clang-format off
        t_gradient
        pos_grad_ultra()
        {
            t_gradient rv;
            rv.add_stop(0.0000, {0  , 7  , 100});
            rv.add_stop(0.1600, {32 , 107, 203});
            rv.add_stop(0.4200, {237, 255, 255});
            rv.add_stop(0.6425, {255, 170, 0});
            rv.add_stop(0.8575, {0  , 2  , 0});
            return rv;
        }
            
            t_gradient
            blues()
            {
                return t_gradient{{
                    {247, 251, 255},
                    {227, 238, 249},
                    {207, 225, 242},
                    {181, 212, 233},
                    {147, 195, 223},
                    {109, 174, 213},
                    {75, 151, 201},
                    {47, 126, 188},
                    {24, 100, 170},
                    {10, 74, 144},
                    {8, 48, 107}
                }};
            }

            t_gradient
            greens()
            {
                return t_gradient{{
                    {247, 252, 245},
                    {232, 246, 227},
                    {211, 238, 205},
                    {183, 226, 177},
                    {151, 212, 148},
                    {115, 195, 120},
                    {77, 175, 98},
                    {47, 152, 79},
                    {21, 127, 59},
                    {3, 100, 41},
                    {0, 68, 27},
                }};
            }

            t_gradient
            greys()
            {
                return t_gradient{{
                    {255, 255, 255},
                    {242, 242, 242},
                    {226, 226, 226},
                    {206, 206, 206},
                    {180, 180, 180},
                    {151, 151, 151},
                    {122, 122, 122},
                    {95, 95, 95},
                    {64, 64, 64},
                    {30, 30, 30},
                    {0, 0, 0},
                }};
            }

            t_gradient
            purples()
            {
                return t_gradient{{
                    {252, 251, 253},
                    {241, 239, 246},
                    {226, 225, 239},
                    {206, 206, 229},
                    {182, 181, 216},
                    {158, 155, 201},
                    {135, 130, 188},
                    {115, 99, 172},
                    {97, 64, 155},
                    {80, 31, 140},
                    {63, 0, 125},
                }};
            }

            t_gradient
            reds()
            {
                return t_gradient{{
                    {255, 245, 240},
                    {254, 227, 214},
                    {253, 201, 180},
                    {252, 170, 142},
                    {252, 138, 107},
                    {249, 105, 76},
                    {239, 69, 51},
                    {217, 39, 35},
                    {187, 21, 26},
                    {151, 11, 19},
                    {103, 0, 13},
                }};
            }

            t_gradient
            oranges()
            {
                return t_gradient{{
                    {255, 245, 235},
                    {254, 232, 211},
                    {253, 216, 179},
                    {253, 194, 140},
                    {253, 167, 98},
                    {251, 141, 61},
                    {242, 112, 29},
                    {226, 86, 9},
                    {196, 65, 3},
                    {159, 51, 3},
                    {127, 39, 4},
                }};
            }
        } // namespace sequential
        
        namespace multi_hue {
            t_gradient
            viridis()
            {
                return t_gradient{{
                    {"#440154"},
                    {"#482475"},
                    {"#414487"},
                    {"#355f8d"},
                    {"#2a788e"},
                    {"#21918c"},
                    {"#22a884"},
                    {"#44bf70"},
                    {"#7ad151"},
                    {"#bddf26"},
                    {"#fde725"},
                }};
            }

            t_gradient
            inferno()
            {
                return t_gradient{{
                    {"#000004"},
                    {"#160b39"},
                    {"#420a68"},
                    {"#6a176e"},
                    {"#932667"},
                    {"#bc3754"},
                    {"#dd513a"},
                    {"#f37819"},
                    {"#fca50a"},
                    {"#f6d746"},
                    {"#fcffa4"},
                }};
            }

            t_gradient
            magma()
            {
                return t_gradient{{
                    {"#000004"},
                    {"#140e36"},
                    {"#3b0f70"},
                    {"#641a80"},
                    {"#8c2981"},
                    {"#b73779"},
                    {"#de4968"},
                    {"#f7705c"},
                    {"#fe9f6d"},
                    {"#fecf92"},
                    {"#fcfdbf"},
                }};
            }

            t_gradient
            plasma()
            {
                return t_gradient{{
                    {"#0d0887"},
                    {"#41049d"},
                    {"#6a00a8"},
                    {"#8f0da4"},
                    {"#b12a90"},
                    {"#cc4778"},
                    {"#e16462"},
                    {"#f2844b"},
                    {"#fca636"},
                    {"#fcce25"},
                    {"#f0f921"},
                }};
            }

            t_gradient
            blue_green()
            {
                return t_gradient{{
                    {247, 252, 253},
                    {232, 246, 249},
                    {213, 239, 237},
                    {183, 228, 218},
                    {143, 211, 193},
                    {104, 194, 163},
                    {73, 177, 127},
                    {47, 153, 89},
                    {21, 127, 60},
                    {3, 100, 41},
                    {0, 68, 27},
                }};
            }

            t_gradient
            blue_purple()
            {
                return t_gradient{{
                    {247, 252, 253},
                    {228, 238, 245},
                    {204, 221, 236},
                    {178, 202, 225},
                    {156, 179, 213},
                    {143, 149, 198},
                    {140, 116, 181},
                    {137, 82, 165},
                    {133, 45, 143},
                    {115, 15, 113},
                    {77, 0, 75},
                }};
            }

            t_gradient
            green_blue()
            {
                return t_gradient{{
                    {247, 252, 240},
                    {229, 245, 223},
                    {211, 238, 206},
                    {189, 229, 191},
                    {158, 217, 187},
                    {123, 203, 196},
                    {88, 183, 205},
                    {57, 156, 198},
                    {29, 126, 183},
                    {11, 96, 161},
                    {8, 64, 129},
                }};
            }

            t_gradient
            orange_red()
            {
                return t_gradient{{
                    {255, 247, 236},
                    {254, 235, 207},
                    {253, 220, 175},
                    {253, 202, 148},
                    {253, 176, 122},
                    {250, 142, 93},
                    {241, 108, 73},
                    {224, 69, 48},
                    {200, 29, 19},
                    {167, 4, 3},
                    {127, 0, 0},
                }};
            }

            t_gradient
            purple_blue_green()
            {
                return t_gradient{{
                    {255, 247, 251},
                    {239, 231, 242},
                    {219, 216, 234},
                    {190, 201, 226},
                    {152, 185, 217},
                    {105, 168, 207},
                    {64, 150, 192},
                    {25, 135, 159},
                    {3, 120, 119},
                    {1, 99, 83},
                    {1, 70, 54},
                }};
            }

            t_gradient
            purple_blue()
            {
                return t_gradient{{
                    {255, 247, 251},
                    {239, 234, 244},
                    {219, 218, 235},
                    {191, 201, 226},
                    {155, 185, 217},
                    {114, 168, 207},
                    {67, 148, 195},
                    {26, 125, 182},
                    {6, 103, 161},
                    {4, 82, 129},
                    {2, 56, 88},
                }};
            }

            t_gradient
            purple_red()
            {
                return t_gradient{{
                    {247, 244, 249},
                    {234, 227, 240},
                    {220, 201, 226},
                    {208, 170, 210},
                    {208, 138, 194},
                    {221, 99, 174},
                    {227, 56, 144},
                    {215, 28, 108},
                    {183, 11, 79},
                    {143, 2, 58},
                    {103, 0, 31},
                }};
            }

            t_gradient
            red_purple()
            {
                return t_gradient{{
                    {255, 247, 243},
                    {253, 228, 225},
                    {252, 207, 204},
                    {251, 181, 188},
                    {249, 147, 176},
                    {243, 105, 163},
                    {224, 62, 152},
                    {192, 23, 136},
                    {153, 3, 124},
                    {112, 1, 116},
                    {73, 0, 106},
                }};
            }

            t_gradient
            yellow_green_blue()
            {
                return t_gradient{{
                    {255, 255, 217},
                    {239, 249, 189},
                    {213, 238, 179},
                    {169, 221, 183},
                    {115, 201, 189},
                    {69, 180, 194},
                    {40, 151, 191},
                    {32, 115, 178},
                    {35, 78, 160},
                    {28, 49, 133},
                    {8, 29, 88},
                }};
            }

            t_gradient
            yellow_green()
            {
                return t_gradient{{
                    {255, 255, 229},
                    {247, 252, 196},
                    {228, 244, 172},
                    {199, 232, 155},
                    {162, 216, 138},
                    {120, 197, 120},
                    {78, 175, 99},
                    {47, 148, 78},
                    {21, 121, 63},
                    {3, 96, 52},
                    {0, 69, 41},
                }};
            }

            t_gradient
            yellow_orange_brown()
            {
                return t_gradient{{
                    {255, 255, 229},
                    {255, 248, 196},
                    {254, 234, 161},
                    {254, 214, 118},
                    {254, 186, 74},
                    {251, 153, 44},
                    {238, 121, 24},
                    {216, 91, 10},
                    {183, 67, 4},
                    {143, 50, 4},
                    {102, 37, 6},
                }};
            }

            t_gradient
            yellow_orange_red()
            {
                return t_gradient{{
                    {255, 255, 204},
                    {255, 240, 169},
                    {254, 224, 135},
                    {254, 201, 101},
                    {254, 171, 75},
                    {253, 137, 60},
                    {250, 92, 46},
                    {236, 48, 35},
                    {211, 17, 33},
                    {175, 2, 37},
                    {128, 0, 38},
                }};
            }

        } // namespace multi_hue
    } // namespace sequential
      // clang-format on

    namespace diverging
    {
        t_gradient
        blue_orange()
        {
            return t_gradient::diverging_gradient(-1., 0., 1.,
                {
                    {5, 48, 97},
                    {34, 101, 163},
                    {75, 148, 196},
                    {143, 194, 221},
                    {205, 227, 238},
                    {242, 240, 235},
                    {253, 221, 179},
                    {248, 182, 100},
                    {221, 132, 31},
                    {178, 90, 9},
                    {127, 59, 8},
                });
        }

        t_gradient
        brown_blue_green()
        {
            return t_gradient::diverging_gradient(-1., 0., 1.,
                {
                    {84, 48, 5},
                    {139, 84, 15},
                    {188, 132, 53},
                    {222, 190, 123},
                    {242, 228, 192},
                    {238, 241, 234},
                    {195, 231, 226},
                    {127, 201, 191},
                    {57, 152, 143},
                    {10, 103, 95},
                    {0, 60, 48},
                });
        }

        t_gradient
        purple_green()
        {
            return t_gradient::diverging_gradient(-1., 0., 1.,
                {
                    {64, 0, 75},
                    {115, 47, 128},
                    {154, 109, 170},
                    {193, 164, 205},
                    {228, 210, 230},
                    {239, 240, 239},
                    {214, 238, 209},
                    {162, 215, 158},
                    {92, 173, 101},
                    {33, 120, 57},
                    {0, 68, 27},
                });
        }

        t_gradient
        pink_yellow_green()
        {
            return t_gradient::diverging_gradient(-1., 0., 1.,
                {
                    {142, 1, 82},
                    {192, 38, 126},
                    {221, 114, 173},
                    {240, 179, 214},
                    {250, 221, 237},
                    {245, 243, 239},
                    {225, 242, 202},
                    {182, 222, 135},
                    {128, 187, 71},
                    {79, 145, 37},
                    {39, 100, 25},
                });
        }

        t_gradient
        purple_orange()
        {
            return t_gradient::diverging_gradient(-1., 0., 1.,
                {
                    {45, 0, 75},
                    {85, 45, 132},
                    {129, 112, 172},
                    {176, 170, 208},
                    {215, 215, 233},
                    {243, 238, 234},
                    {253, 221, 179},
                    {248, 182, 100},
                    {221, 132, 31},
                    {178, 90, 9},
                    {127, 59, 8},
                });
        }

        t_gradient
        red_blue()
        {
            return t_gradient::diverging_gradient(-1., 0., 1.,
                {
                    {103, 0, 31},
                    {172, 32, 47},
                    {213, 96, 80},
                    {241, 163, 133},
                    {251, 215, 196},
                    {242, 239, 238},
                    {205, 227, 238},
                    {143, 194, 221},
                    {75, 148, 196},
                    {34, 101, 163},
                    {5, 48, 97},
                });
        }

        t_gradient
        red_grey()
        {
            return t_gradient::diverging_gradient(-1., 0., 1.,
                {
                    {103, 0, 31},
                    {172, 32, 47},
                    {213, 96, 80},
                    {241, 163, 133},
                    {252, 216, 197},
                    {250, 244, 241},
                    {223, 223, 223},
                    {184, 184, 184},
                    {134, 134, 134},
                    {78, 78, 78},
                    {26, 26, 26},
                });
        }

        t_gradient
        red_yellow_blue()
        {
            return t_gradient::diverging_gradient(-1., 0., 1.,
                {
                    {165, 0, 38},
                    {212, 50, 44},
                    {241, 110, 67},
                    {252, 172, 100},
                    {254, 221, 144},
                    {250, 248, 193},
                    {220, 241, 236},
                    {171, 214, 232},
                    {117, 171, 208},
                    {74, 116, 180},
                    {49, 54, 149},
                });
        }

        t_gradient
        red_yellow_green()
        {
            return t_gradient::diverging_gradient(-1., 0., 1.,
                {
                    {165, 0, 38},
                    {212, 50, 44},
                    {241, 110, 67},
                    {252, 172, 99},
                    {254, 221, 141},
                    {249, 247, 174},
                    {215, 238, 142},
                    {164, 216, 110},
                    {100, 188, 97},
                    {34, 150, 79},
                    {0, 104, 55},
                });
        }

        t_gradient
        spectral()
        {
            return t_gradient::diverging_gradient(-1., 0., 1.,
                {
                    {158, 1, 66},
                    {209, 60, 75},
                    {240, 112, 74},
                    {252, 172, 99},
                    {254, 221, 141},
                    {251, 248, 176},
                    {224, 243, 161},
                    {169, 221, 162},
                    {105, 189, 169},
                    {66, 136, 181},
                    {94, 79, 162},
                });
        }

    } // namespace diverging

    namespace cyclical
    {
        t_gradient
        rainbow()
        {
            return t_gradient{{
                {"#6e40aa"},
                {"#bf3caf"},
                {"#fe4b83"},
                {"#ff7847"},
                {"#e2b72f"},
                {"#aff05b"},
                {"#52f667"},
                {"#1ddfa3"},
                {"#23abd8"},
                {"#4c6edb"},
                {"#6e40aa"},
            }};
        }

        t_gradient
        sinebow()
        {
            return t_gradient{{
                {"#ff4040"},
                {"#e78d0b"},
                {"#a7d503"},
                {"#58fc2a"},
                {"#18f472"},
                {"#00bfbf"},
                {"#1872f4"},
                {"#582afc"},
                {"#a703d5"},
                {"#e70b8d"},
                {"#ff4040"},
            }};
        }
    } // namespace cyclical
} // namespace color
} // namespace perspective
