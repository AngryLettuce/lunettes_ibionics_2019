function [laserplot1, laserplot2, normplot1, normplot2, memsplot2, fig_h] = plotSetup2(Vlm, N, Vmw, mems, wall, wallcorner, pixMat)

    fig_h = figure('keypressfcn',@keypressed);
%     fig_h.WindowState = 'maximized';
    subplot(1,2,1)
    hold on
    plot3([Vlm(1) 0],[Vlm(2) 0],[Vlm(3) 0]) %laser to MEMS
    laserplot1 = plot3([0 Vmw(1)],[0 Vmw(2)],[0 Vmw(3)], 'green'); %MEMS to wall
    normplot1  = plot3([0 N(1)],[0 N(2)],[0 N(3)]); %Normal
    surf(mems.X,mems.Y,mems.Z,'FaceColor','white') %mems surface
%     fill3(wall(:,1)', wall(:,2)', wall(:,3)', 'yellow') %wall surface
    

%     plot3(pixMat(:,1), pixMat(:, 2:end), Vmw(3) * ones(length(pixMat(:,1)),1)  ,'.') 
    fill3(wallcorner(:,1)', wallcorner(:,2)', wallcorner(:,3)', 'black') %wall surface
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
    title('Vue de face avec mur')
    hold off    
    

    subplot(1,2,2)
    hold on
    plot3([Vlm(1) 0],[Vlm(2) 0],[Vlm(3) 0]) %laser to MEMS
    laserplot2 = plot3([0 Vmw(1)],[0 Vmw(2)],[0 Vmw(3)], 'green'); %MEMS to wall
    normplot2  = plot3([0 N(1)],[0 N(2)],[0 N(3)]); %Normal
    memsplot2 = surf(mems.X,mems.Y,mems.Z,'FaceColor','white'); %mems surface
    fill3(wallcorner(:,1)', wallcorner(:,2)', wallcorner(:,3)', 'black') %wall surface

    drawGlasses(0);
%     head();
%     fullbody();

    % adjusting various graph view and options
    grid on
    grid minor
    xlabel('X')
    ylabel('Y')
    zlabel('Z')
    xlim([-50 50])
    ylim([-60 15])
    zlim([-10 50])

    daspect([1 1 1])
    view(90,-40)
    title('Vue des lunettes et du MEMS')
    hold off 
end