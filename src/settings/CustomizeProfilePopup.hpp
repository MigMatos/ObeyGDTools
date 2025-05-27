class CustomizeProfilePopup : public Popup<> {
protected:
    ColorChannelSprite* buttonSprite = nullptr;
    ccColor4B colorPicked = {0, 0, 0, 255};

    bool setup() override;

    void openColorPopup(CCObject* sender);
    void onColorUpdated(ccColor4B const& color);

public:
    static CustomizeProfilePopup* create();
    ccColor4B getCurrentColor() const { return colorPicked; }
    void setCurrentColor(ccColor4B const& color);
};
