#import <UIKit/UIKit.h>

#import "paycb.h"
#import "HNPayModule.h"


class MultiPayDelegate {
public:
    virtual void handleMultiPayStart(HN::PayModule* pay) = 0;
    virtual void handleMultiPayCancel() = 0;
};

@interface PayViewController : UIViewController
{
    
}

@property (retain, nonatomic) IBOutlet UIButton *ButtonPay1;
@property (retain, nonatomic) IBOutlet UIButton *ButtonPay2;
@property (retain, nonatomic) IBOutlet UIButton *ButtonClose;

- (IBAction) ButtonPay1Click:(id)sender;
- (IBAction) ButtonPay2Click:(id)sender;
- (IBAction) ButtonCloseClick:(id)sender;


- (void) start:(UIViewController*) viewController MultiPayDelegate:(MultiPayDelegate*)multiPayDelegate;

- (void) showPay;

- (void) setPayPriority:(HN::PayModule*)first firstName:(NSString*)firstName
                 second:(HN::PayModule*)second secondName:(NSString*)secondName;

@end

