
void rectangle_fill(Rect rect, uint16_t color)
{
    WAWO_LOG("rectangle_fill {%d, %d}, {%d, %d} %X} ", rect.point.x, rect.point.y, rect.size.width, rect.size.height, color);
    auto &display = get_instance<wawo::features::display::Ping_pong_display>();
    display.start_flush(rect);
    uint32_t remain_size = rect.size.height * rect.size.width * BYTES_PER_PIXEL;
    do
    {
        auto const buffer = display.get_flush_data();
        Ensures_expression(buffer.first == Display::Result::success, goto RETURNED);
        auto const span = make_span(reinterpret_cast<uint16_t *>(buffer.second.data()), buffer.second.size() / 2);
        fill(span.begin(), span.end(), static_cast<uint16_t>(color));
        if (remain_size < buffer.second.size())
        {
            remain_size = 0;
        }
        else
        {
            remain_size -= buffer.second.size();
        }
        auto const flush_result = display.ping_pong_flush(buffer.second, remain_size <= 0);
        Ensures_expression(flush_result == Display::Result::success, goto RETURNED);
    } while (remain_size > 0);
RETURNED:
    if (remain_size > 0)
    {
        auto const flush_result = display.ping_pong_flush(gsl::span<uint8_t>{}, true);
        Ensures_expression(flush_result == Display::Result::success, goto RETURNED);
    }
}
