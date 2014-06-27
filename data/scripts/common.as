namespace Console
{
    enum Level
    {
        Message,
        Info,
        Warning,
        Error,
        Fatal
    }
}

float interpolate(const float a, const float b, const float bias)
{
	return a + ((b - a) * bias);
}
