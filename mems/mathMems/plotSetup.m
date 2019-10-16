function [laserplot, normplot, fig_h] = plotSetup(Vlm, N, Vmw, mems, wall, caption)

    fig_h = figure('keypressfcn',@keypressed);
%     fig_h.WindowState = 'maximized';
    hold on
    plot3([Vlm(1) 0],[Vlm(2) 0],[Vlm(3) 0]) %laser to MEMS
    laserplot = plot3([0 Vmw(1)],[0 Vmw(2)],[0 Vmw(3)], 'green'); %MEMS to wall
    normplot  = plot3([0 N(1)],[0 N(2)],[0 N(3)]); %Normal
    surf(mems.X,mems.Y,mems.Z,'FaceColor','white') %mems surface
    fill3(wall(:,1)', wall(:,2)', wall(:,3)', 'yellow') %wall surface

    %show laser and wall coordinates
%     aX = evalin('base', 'aX');
%     aY = evalin('base', 'aY');
%     text(30,30 ,0,['X Angle: ' num2str(aX), newline 'Y Angle: ' num2str(aY)]);

    drawGlasses(0);
%     head();
    fullbody();

    % adjusting various graph view and options
    grid on
    grid minor
    xlabel('X')
    ylabel('Y')
    zlabel('Z')
    daspect([1 1 1])
    view(90,-90)
    title(caption)
%     legend('Laser to MEMS','MEMS to Wall','Normal','MEMS','Wall','GlassR','GlassL','GlassF','Laser Diode')
    hold off    
end