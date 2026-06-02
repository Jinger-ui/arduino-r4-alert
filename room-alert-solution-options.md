# Cross-Room Alert / Status Dashboard Solution Notes

## 1. Purpose

This note compares three implementation options for delivering alerts or status notifications from the control room to other rooms. The goal is to balance:

- Deployment speed
- Hardware and setup cost
- Ease of maintenance
- Professional presentation
- Long-term reliability

---

## 2. Executive Summary

| Option | Solution | Primary Output | Cost Level | Deployment Speed | Maintenance Difficulty | Flexibility | Best Fit |
|---|---|---|---|---|---|---|---|
| Option 1 | Bluetooth Audio Solution | Voice alerts / alarm sounds | Low | Fast | Low | Low | Low-budget, sound-only notification |
| Option 2 | Raspberry Pi Pico LED Dashboard | LED indicators / expandable display | Low to Medium | Medium | High | Very High | Custom dashboard and visual signaling |
| Option 3 | Programmable Smart Lights | Light color changes / optional sound | Medium | Fastest | Medium | Medium | Professional-looking, quick deployment |

---

## 3. Side-by-Side Comparison

### 3.1 Functional Comparison

| Dimension | Option 1: Bluetooth Audio | Option 2: Raspberry Pi Pico | Option 3: Smart Lights |
|---|---|---|---|
| Core concept | Sync audio across rooms with Bluetooth speakers | Use Pico W with MicroPython to drive LEDs or displays | Convert control room signals into smart light API commands |
| Notification type | Audio only | Visual dashboard / LED logic | Light-based alert, sometimes with sound |
| Customization level | Low | Highest | Medium |
| Professional appearance | Basic | Can be highly professional if well designed | Strong out-of-the-box presentation |
| Technical complexity | Low | High | Medium |
| Expandability | Low | Very high | Medium |

### 3.2 Maintenance Comparison

| Dimension | Option 1: Bluetooth Audio | Option 2: Raspberry Pi Pico | Option 3: Smart Lights |
|---|---|---|---|
| Day-to-day maintenance | Very easy | Difficult | Moderate |
| Can non-technical staff handle basic issues? | Yes | Usually no | Partially |
| Common failure points | Bluetooth disconnects, weak wall penetration | Script failure, loose wiring, flashing issues | API bugs, network instability, device offline |
| Hardware replacement | Easy | Medium | Easy |
| Hidden maintenance cost | Low | High | Medium |
| Long-term reliability | Fair | Depends heavily on support quality | Balanced |

---

## 4. Option Details

## 4.1 Option 1: Bluetooth Audio Solution

### Concept

Use Bluetooth speakers to sync audio from the control room to nearby or separate rooms, so alerts can be delivered as voice prompts or alarm sounds.

### Key Actions

- Check whether the control room hardware already supports Bluetooth.
- If not, purchase a USB Bluetooth transmitter.
- Test signal stability across walls and room distance before final deployment.
- Choose speakers that can stay powered on and reconnect easily after a disconnect.

### Maintenance

This is the easiest option for non-technical teams to maintain.

- If Bluetooth disconnects, staff only need to re-pair the speaker.
- If a speaker fails, it can simply be replaced with a new one.
- No hardware repair or code maintenance is usually required.

### Advantages

- Lowest cost
- Fast deployment
- Very easy for non-technical staff to understand
- Easy device replacement

### Limitations

- Audio only, with no visual dashboard capability
- Bluetooth signal may drop or weaken through walls
- Less suitable for complex or multi-state status communication

### Best Use Case

Best for teams that only need simple voice or sound alerts and have little or no on-site technical support.

---

## 4.2 Option 2: Raspberry Pi Pico (LED Dashboard)

### Concept

Use a Raspberry Pi Pico W running MicroPython scripts to control LED strips or other visual indicators. This can later be extended into a more advanced display-based dashboard if needed.

### Hardware

- Raspberry Pi Pico W
- LED strips or indicator modules
- Micro-USB cable
- Optional enclosure, power accessories, and display expansion parts

### Key Actions

- Build the signal-to-light logic in MicroPython.
- Define clear alert states, such as color, blinking mode, or grouped indicators.
- Prepare a clean wiring layout and document all connections.
- Keep at least one spare board or module if this option will be used in production.

### Maintenance

This is the most challenging option for low-tech teams.

- If the script fails, non-technical staff usually cannot troubleshoot it.
- If wiring becomes loose, diagnosis may still require technical support.
- Any logic change often requires code updates and reflashing.

### Advantages

- Highest flexibility
- Best option for a true custom dashboard
- Low hardware cost with strong expansion potential

### Limitations

- High hidden maintenance cost
- Heavy dependence on technical support
- Greater risk during long-term use if documentation is weak

### Best Use Case

Best when a specific professional dashboard is required and there is reliable technical support available for setup, troubleshooting, and future changes.

---

## 4.3 Option 3: Programmable Smart Lights

### Concept

Deploy off-the-shelf smart lights and use software to convert control room signals into API commands that trigger color, flashing, or sound changes.

### Procurement Note

Buying from Taobao plus air freight is significantly cheaper than purchasing the same category of products from Shopee or Lazada in many cases. Pricing advantage should still be balanced against delivery time, availability, and replacement convenience.

### Key Actions

- Select smart lights that support stable API or programmable control.
- Build a software bridge from control room signals to lighting commands.
- Test network stability, reconnection behavior, and device response time.
- Standardize replacement models so failed hardware can be swapped quickly.

### Maintenance

This option is more manageable than a custom electronics build, but still depends on software stability.

- Non-technical staff can usually replace broken bulbs or devices.
- Software/API issues may still require remote technical support.
- Stable network conditions are important for reliable operation.

### Advantages

- Fastest deployment
- Professional-looking result
- Easier hardware replacement than custom electronics
- Good balance between polish and maintenance effort

### Limitations

- Less flexible than Pico for complex dashboard logic
- Still depends on software integration and network quality
- May require remote reset or troubleshooting if the system becomes unstable

### Best Use Case

Best for teams that want a polished and professional alert system with relatively simple maintenance and quicker deployment.

---

## 5. Decision Matrix

| Situation | Recommended Option | Reason |
|---|---|---|
| No on-site technical support | Option 1 or Option 3 | Easier replacement and lower support dependency |
| Need a professional custom dashboard | Option 2 | Best visual customization and logic control |
| Lowest-cost quick rollout | Option 1 | Cheapest and simplest to deploy |
| Best long-term balance | Option 3 | Strong tradeoff between professional appearance and maintenance ease |

---

## 6. Final Recommendation

### If there is no on-site technical support

Choose **Option 1** or **Option 3**.

- Choose **Option 1** if only sound-based alerts are needed and the budget is very limited.
- Choose **Option 3** if a more professional and polished result is important.

### If a custom dashboard is mandatory

Choose **Option 2**, but only if the project also includes:

- A wiring guide
- A user manual
- Spare hardware
- Technical support for reflashing and repairs

### Best overall balance

**Option 3** is the most balanced solution for long-term use. It offers a more professional result than Bluetooth audio while avoiding much of the maintenance burden and technical fragility of a custom Pico-based system.

---

## 7. One-Line Summary

- **Option 1:** Cheapest and simplest
- **Option 2:** Most flexible and most customizable
- **Option 3:** Best overall balance for long-term deployment
