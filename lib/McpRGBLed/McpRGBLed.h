#include <stdint.h>
#include <MCP.h>

enum class RGBLedColor { BLACK, RED, GREEN, BLUE, YELLOW, CYAN, PURPLE, WHITE };

class McpRGBLed
{
public:
  McpRGBLed(MCP* mcp, uint8_t pinR, uint8_t pinG, uint8_t pinB, bool commonAnode = true);
  void setColor(RGBLedColor color);

private:
  MCP* _mcp;

  uint8_t _pinR; 
  uint8_t _pinG; 
  uint8_t _pinB;
  bool _commonAnode;
};